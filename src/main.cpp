// https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <iostream>
#include <chrono>
#include <thread>

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

#if 0
int main1(void)
{
    Vec3 a = vec3(1.0f, 2.0f, 3.0f);
    Vec3 b = vec3(4.0f, 5.0f, 6.0f);

    Vec3 res = a + b;
    Vec3 res2 = {};
    res2 += a;
    res2 += b;

    assert(res == res2 && "Math is worng!");

    Vec3 c = -res;

    Vec3 d = { 1,2,3 };
    d *= 2.0f;

    Vec3 e = { 3,4,5 };
    float e_len = len(e);

    {
        auto res = len(e);
        auto res2 = unit(e);
        auto res3 = len(res2);
        int k = 0;
    }
    return 0;
}
#endif

struct World
{
    Sphere spheres[1024] = {};
    size_t sphere_count = 0;
};

World global_world;


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

static void write_color(FILE* fp, Color pixel_color, int samples_per_pixel)
{
    float r = pixel_color.r;
    float g = pixel_color.g;
    float b = pixel_color.b;

    // Divide the color by the number of samples.

    float scale = 1.0f / (float)samples_per_pixel;
    r = sqrtf(scale * r);
    g = sqrtf(scale * g);
    b = sqrtf(scale * b);

    fprintf(fp, "%d %d %d\n",
        (int)(256.0f * clamp(r, 0.0f, 0.999f)), 
        (int)(256.0f * clamp(g, 0.0f, 0.999f)), 
        (int)(256.0f * clamp(b, 0.0f, 0.999f)));
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
    int depth)
{
    Hit_Record rec = {};

    if (depth <= 0)
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
static int g_total_scanlines = 0;
static int g_scanlines_processed = 0;

static void print_progress()
{
    using namespace std::chrono_literals;

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

        std::this_thread::sleep_for(1s);
    }

    auto end = chrono::steady_clock::now();
    auto total_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    printf("Done! (%lldms)\n", total_time);
}


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

    int stop = 0;
}

int main(void)
{
    setbuf(stdout, NULL);

    FILE* fp = fopen("output.ppm", "wb");

    // Image 

    float aspect_ratio = 3.0f / 2.0f;
    int image_width = 240;
    int image_height = (int)((float)image_width / aspect_ratio);
    int samples_per_pixel = 50;
    int max_depth = 25;
    
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
        aspect_ratio, 
        aperture, 
        dist_to_focus);

    // Render

    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", image_width, image_height);
    fprintf(fp, "255\n");

    std::thread progress_thread(print_progress);
    
    g_total_scanlines = image_height;

    for (int row = image_height - 1;
        row >= 0;
        --row)
    {
        for (int col = 0;
            col < image_width;
            ++col)
        {
            Color pixel_color = vec3(0.0f);

            for (int s = 0;
                s < samples_per_pixel;
                ++s)
            {
                float u = ((float)col + random_float()) / ((float)image_width - 1.0f);
                float v = ((float)row + random_float()) / ((float)image_height - 1.0f);
            
                Ray ray = get_ray_from_camera(&cam, u, v);

                pixel_color += ray_color(&ray, &global_world, max_depth);
            }
            
            write_color(fp, pixel_color, samples_per_pixel);
        }

        ++g_scanlines_processed;
    }
    
    g_done_rendering = true;
    if (progress_thread.joinable())
        progress_thread.join();

    fclose(fp);

    return 0;
}