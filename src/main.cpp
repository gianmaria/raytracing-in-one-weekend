// https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "vec3.h"
#include "ray.h"

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

void write_color(FILE* fp, Color color)
{
    int ir = (int)(255.999 * color.r);
    int ig = (int)(255.999 * color.g);
    int ib = (int)(255.999 * color.b);

    fprintf(fp, "%d %d %d\n",
        ir, ig, ib);
}

float hit_sphere(Point3 center, float radius, Ray r)
{
    Vec3 oc = r.origin - center;
    float a = dot(r.direction, r.direction);
    float b = 2.0f * dot(oc, r.direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4.0f * a * c;
    
    if (discriminant < 0.0f)
        return -1.0f;
    else
        return (-b - sqrtf(discriminant) ) / (2.0f * a);
}

Color ray_color(Ray r)
{
    float t = hit_sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, r);

    if (t > 0.0f)
    {
        Vec3 N = unit_vec(r.at(t) - vec3(0.0f, 0.0f, -1.0f));
        return 0.5f * vec3(N.r + 1.0f, 
                           N.g + 1.0f, 
                           N.b + 1.0f);
    }

    Vec3 unit_direction = unit_vec(r.direction);
    t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * vec3(1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}



int main(void)
{

    hit_sphere(vec3(), 0.0f, ray(vec3(), vec3()));

    FILE* fp = fopen("output.ppm", "wb");

    float aspect_ratio = 16.0f / 9.0f;
    int image_width = 400;
    int image_height = (int)((float)image_width / aspect_ratio);

    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height;
    float focal_length = 1.0f;

    Point3 origin = vec3(0.0f);
    Vec3 horizontal = vec3(viewport_width, 0.0f, 0.0f);
    Vec3 vertical = vec3(0.0f, viewport_height, 0.0f);
    Vec3 lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - vec3(0.0f, 0.0f, focal_length);


    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", image_width, image_height);
    fprintf(fp, "255\n");

    for (int row = image_height - 1;
        row >= 0;
        --row)
    {
        //printf("Scanlines remaining: %d\n", row);

        for (int col = 0;
            col < image_width;
            ++col)
        {
            float u = (float)col / ((float)image_width - 1.0f);
            float v = (float)row / ((float)image_height - 1.0f);
            
            Ray r = ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            Color pixel_color = ray_color(r);
            write_color(fp, pixel_color);
        }
    }


    fclose(fp);

    return 0;
}