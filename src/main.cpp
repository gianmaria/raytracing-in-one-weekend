// https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <cassert>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string_view>
#include <vector>
#include <future>

namespace chrono = std::chrono;
using std::cout;
using std::endl;

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "shpere.h"
#include "rtweekend.h"
#include "camera.h"
#include "material.h"


struct World
{
    Sphere spheres[1024] = {};
    size_t sphere_count = 0;
};

World global_world;


struct alignas(1) Pixel
{
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;

    Pixel() = default;

    Pixel(unsigned char r, unsigned char g, unsigned char b) :
        r(r), g(g), b(b)
    {}
};

struct Image
{
    unsigned m_width = 0;
    float m_aspect_ratio = 0.0f;
    unsigned m_height = 0;
    unsigned m_samples_per_pixel = 0;
    unsigned m_max_depth = 0;
    std::vector<Pixel> m_pixels;

    Image(unsigned width, float aspect_ratio, unsigned samples_per_pixel, unsigned max_depth) :
        m_width(width), m_aspect_ratio(aspect_ratio), m_samples_per_pixel(samples_per_pixel),
        m_max_depth(max_depth)
    {
        m_height = (unsigned)((float)m_width / m_aspect_ratio);
        m_pixels.resize(m_width * m_height);
    }

    Pixel& at(unsigned row, unsigned col)
    {
        if (row < m_height &&
            col < m_width)
        {
            unsigned index = m_width * row + col;
            return m_pixels[index];
        }

        cout << "[ERROR] row or col out of bounds" << endl;
        exit(-1);
    }

    float aspect_ratio()
    {
        float ar = (float)m_width / (float)m_height;
        return ar;
    }

    Pixel& operator[](unsigned index)
    {
        if (index < m_width * m_height)
        {
            return m_pixels[index];
        }

        cout << "[ERROR] index out of bounds" << endl;
        exit(-1);
    }

    bool save(const std::string_view& filename, long long total_time_ms)
    {
        try
        {
            std::fstream fs;
            fs.open(filename.data(), std::fstream::out | std::fstream::binary);
        
            if (!fs.is_open())
            {
                return false;
            }

            fs << "P3\n";
            fs << m_width << " " << m_height << "\n";
            fs << "255\n";
            fs << "# took " << total_time_ms << "ms\n";

            for (unsigned row_ = 0;
                row_ < m_height;
                ++row_)
            {
                unsigned row = m_height - 1 - row_;

                for (unsigned col = 0;
                    col < m_width;
                    ++col)
                {
                    Pixel& p = at(row, col);
                    fs << (int)p.r << " " << (int)p.g << " " << (int)p.b << "\n";
                }
            }

            fs.close();

            return true;
        } 
        catch (std::ios_base::failure& e)
        {
            cout << "[ERROR] cannot open file '" << filename << "' for writing: " <<
                e.what() << "(" << e.code() << ")" << endl;
            return false;
        }
    }

};

static bool add_sphere_to_world(World* world, const Sphere* new_sphere)
{
    unsigned max_spheres = ArrayCount(world->spheres);

    if (world->sphere_count < max_spheres)
    {
        Sphere* sphere = &world->spheres[world->sphere_count];
        *sphere = *new_sphere;
        ++world->sphere_count;
        return true;
    }
    else
    {
        return false;
    }
}

static void write_color(Image* image, unsigned row, unsigned col, 
                        Color pixel_color, unsigned samples_per_pixel)
{
    float r = pixel_color.r;
    float g = pixel_color.g;
    float b = pixel_color.b;

    // Divide the color by the number of samples.

    float scale = 1.0f / (float)samples_per_pixel;
    r = sqrtf(scale * r);
    g = sqrtf(scale * g);
    b = sqrtf(scale * b);

    Pixel p(
        (unsigned char)(256.0f * clamp(r, 0.0f, 0.999f)),
        (unsigned char)(256.0f * clamp(g, 0.0f, 0.999f)),
        (unsigned char)(256.0f * clamp(b, 0.0f, 0.999f)));

    image->at(row, col) = p;
}

static bool ray_hit_object_in_world(
    const Ray* ray, 
    const World* world, 
    Hit_Record* rec)
{
    Hit_Record temp_rec = {};
    bool hit_anything = false;
    float closest_so_far = INF;

    // check spheres
    for (size_t i = 0;
        i < world->sphere_count;
        ++i)
    {
        const Sphere* sphere = &world->spheres[i];

        if (ray_hit_sphere(sphere, ray, 0.001f, closest_so_far, &temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }

    return hit_anything;
}

static Color ray_color(
    const Ray* ray, 
    const World* world, 
    unsigned depth)
{
    Hit_Record rec = {};

    if (depth == 0)
    {
        return vec3(0.0f);
    }

    if (ray_hit_object_in_world(ray, world, &rec))
    {
        Ray scattered = {};
        Color attenuation = {};

        bool res = scatter(rec.material, ray, &rec, &attenuation, &scattered);
        
        if (res)
            return attenuation * ray_color(&scattered, world, depth - 1);
        else
            return vec3(0.0f, 0.0f, 0.0f);
    }

    Vec3 unit_direction = unit_vector(ray->direction);
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}


static bool g_done_rendering = false;
static unsigned g_total_scanlines = 0;
static unsigned g_scanlines_processed = 0;



static void init_random_scene()
{
    Material* ground_material = (Material*)malloc(sizeof(Material));
    ground_material->type = Material_Type::lambertian;
    ground_material->lambertian.color = color(0.5f, 0.5f, 0.5f);

    {
        Sphere s = sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, ground_material);
        add_sphere_to_world(&global_world, &s);
    }

    for (int a = -11; a < 11; ++a) 
    {
        for (int b = -11; b < 11; ++b) 
        {
            float choose_mat = random_float();

            Point3 center = vec3(
                (float)a + 0.9f * random_float(), 
                0.2f, 
                (float)b + 0.9f * random_float());

            if (length(center - vec3(4.0f, 0.2f, 0.0f)) > 0.9f)
            {
                if (choose_mat < 0.8f) 
                {
                    // diffuse
                    Color albedo = random_color() * random_color();
                    
                    Material* sphere_material = (Material*)malloc(sizeof(Material));
                    sphere_material->type = Material_Type::lambertian;
                    sphere_material->lambertian.color = albedo;

                    Sphere s = sphere(center, 0.2f, sphere_material);

                    add_sphere_to_world(&global_world, &s);
                }
                else if (choose_mat < 0.95f) 
                {
                    // metal
                    Color albedo = random_color(0.5f, 1.0f);
                    float fuzz = random_float(0.0f, 0.5f);

                    Material* sphere_material = (Material*)malloc(sizeof(Material));
                    sphere_material->type = Material_Type::metal;
                    sphere_material->metal.color = albedo;
                    sphere_material->metal.fuzz = fuzz;

                    Sphere s = sphere(center, 0.2f, sphere_material);

                    add_sphere_to_world(&global_world, &s);
                }
                else 
                {
                    // glass
                    Material* sphere_material = (Material*)malloc(sizeof(Material));
                    sphere_material->type = Material_Type::dielectric;
                    sphere_material->dielectric.ir = 1.5f;

                    Sphere s = sphere(center, 0.2f, sphere_material);

                    add_sphere_to_world(&global_world, &s);
                }
            }
        }
    }

    {
        Material* material1 = (Material*)malloc(sizeof(Material));
        material1->type = Material_Type::dielectric;
        material1->dielectric.ir = 1.5f;

        Sphere s = sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, material1);

        add_sphere_to_world(&global_world, &s);
    }

    {
        Material* material2 = (Material*)malloc(sizeof(Material));
        material2->type = Material_Type::lambertian;
        material2->lambertian.color = vec3(0.4f, 0.2f, 0.1f);

        Sphere s = sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, material2);

        add_sphere_to_world(&global_world, &s);
    }

    {
        Material* material3 = (Material*)malloc(sizeof(Material));
        material3->type = Material_Type::metal;
        material3->metal.color = vec3(0.7f, 0.6f, 0.5f);
        material3->metal.fuzz = 0.0f;

        Sphere s = sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, material3);

        add_sphere_to_world(&global_world, &s);
    }
}

static void render_scene(Image& image, const Camera& cam)
{
    for (unsigned row_ = 0;
        row_ < image.m_height;
        ++row_)
    {
        unsigned row = image.m_height - 1 - row_;

        for (unsigned col = 0;
            col < image.m_width;
            ++col)
        {
            Color pixel_color = vec3(0.0f);

            for (unsigned s = 0;
                s < image.m_samples_per_pixel;
                ++s)
            {
                float u = ((float)col + random_float()) / ((float)image.m_width - 1.0f);
                float v = ((float)row + random_float()) / ((float)image.m_height - 1.0f);

                Ray ray = get_ray_from_camera(&cam, u, v);

                pixel_color += ray_color(&ray, &global_world, image.m_max_depth);
            }

            write_color(&image, row, col, pixel_color, image.m_samples_per_pixel);
        }

        ++g_scanlines_processed;
    }

    g_done_rendering = true;
}

int main(void)
{
    setbuf(stdout, NULL);

    // Image

    Image image(240, 3.0f / 2.0f, 50, 25);

#define GOTTA_GO_SLOW 0

#if GOTTA_GO_SLOW
    image_width = 1200;
    image_height = (int)((float)image_width / aspect_ratio);
    samples_per_pixel = 500;
    max_depth = 50;
#endif

    // World 
    init_random_scene();

    // Camera

    Point3 lookfrom = vec3(13.0f, 2.0f, 3.0f);
    Point3 lookat = vec3(0.0f, 0.0f, 0.0f);
    Vec3 vup = vec3(0.0f, 1.0f, 0.0f);
    float dist_to_focus = 10.0f;
    float aperture = 0.1f;

    Camera cam = camera(
        lookfrom, 
        lookat, 
        vup, 
        20.0f, 
        image.aspect_ratio(),
        aperture, 
        dist_to_focus);


    g_total_scanlines = image.m_height;
    
    // Render
    
    std::thread worker(render_scene, std::ref(image), std::ref(cam));
    

    // Print progress

    chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();

    printf("Begin rendering\n");

    while (!g_done_rendering)
    {
        float perc = (float)g_scanlines_processed / (float)g_total_scanlines * 100.0f;
        auto now = chrono::steady_clock::now();
        long long elapsed_ms =
            chrono::duration_cast<chrono::milliseconds>(now - start).count();

        long long sec = elapsed_ms / 1000;

        printf("[%5.2f%%] %llds\n", perc, sec);

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    auto end = chrono::steady_clock::now();
    auto total_time_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    printf("Done! (%lldms)\n", total_time_ms);

    if (worker.joinable())
        worker.join();
    
    std::string_view filename("output.ppm");

    if (!image.save(filename, total_time_ms))
    {
        cout << "[ERROR] cannot save image '" << filename << "' on disk" << endl;
    }

    return 0;
}
