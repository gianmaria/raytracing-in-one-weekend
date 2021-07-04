#pragma once

#include "vec3.h"
#include "ray.h"

struct Camera
{
    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;

};

Camera default_camera();

Ray get_ray_from_camera(Camera* camera, float u, float v);
