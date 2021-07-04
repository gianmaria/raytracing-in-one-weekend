// https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <iostream>
#include <chrono>

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "shpere.h"
#include "rtweekend.h"
#include "camera.h"

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
    Sphere spheres[2];
};

void write_color(FILE* fp, Color pixel_color, int samples_per_pixel)
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

bool ray_hit_object_in_world(Ray* ray, World* world, Hit_Record* rec)
{
    Hit_Record temp_rec = {};
    bool hit_anything = false;
    float closest_so_far = INF;

    // check spheres
    for (size_t i = 0;
        i < ArrayCount(world->spheres);
        ++i)
    {
        Sphere* sphere = &world->spheres[i];

        if (ray_hit_sphere(sphere, ray, 0.001f, closest_so_far, &temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }

    return hit_anything;
}

Color ray_color(Ray* ray, World* world, int depth)
{
    Hit_Record rec = {};

    if (depth <= 0)
    {
        return vec3(0.0f);
    }

    if (ray_hit_object_in_world(ray, world, &rec))
    {
        Point3 target = rec.point + random_in_hemisphere(rec.normal);
        
        Ray new_ray = {};
        new_ray.origin = rec.point;
        new_ray.direction = target - rec.point;

        return 0.5f * ray_color(&new_ray, world, depth - 1);
    }

    Vec3 unit_direction = unit_vec(ray->direction);
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * vec3(1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}





int main(void)
{
    FILE* fp = fopen("output.ppm", "wb");

    // Image 

    float aspect_ratio = 16.0f / 9.0f;
    int image_width = 400;
    int image_height = (int)((float)image_width / aspect_ratio);
    int samples_per_pixel = 100;
    int max_depth = 50;
    
#define GOTTA_GO_FAST 0

#if GOTTA_GO_FAST
    samples_per_pixel = 50;
    max_depth = 25;
#endif

    // World 

    World world = {};

    world.spheres[0] = sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
    world.spheres[1] = sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f);


    // Camera

    Camera cam = default_camera();

    // Render

    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", image_width, image_height);
    fprintf(fp, "255\n");

    std::chrono::time_point<std::chrono::steady_clock> start =
        std::chrono::steady_clock::now();

    for (int row = image_height - 1;
        row >= 0;
        --row)
    {
        //printf("Scanlines remaining: %d\n", row);

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

                pixel_color += ray_color(&ray, &world, max_depth);
            }
            
            write_color(fp, pixel_color, samples_per_pixel);
        }
    }

    auto end = std::chrono::steady_clock::now();
    std::cout << "Took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    fclose(fp);

    return 0;
}