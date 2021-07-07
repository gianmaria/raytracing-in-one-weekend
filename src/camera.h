#pragma once

#include "vec3.h"
#include "ray.h"

struct Camera
{
    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    float lens_radius;
};

Camera camera(
    Point3 lookfrom,
    Point3 lookat,
    Vec3 vup,
    float vfov, // vertical field-of-view in degrees
    float aspect_ratio,
    float aperture,
    float focus_distance);


Ray get_ray_from_camera(const Camera* camera, float s, float t);
