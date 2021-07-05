#pragma once

#include "ray.h"

struct Material;

struct Hit_Record
{
    Point3 point;
    Vec3 normal;
    float t;
    bool front_face;
    Material* material;

    inline void set_face_normal(const Ray* ray, Vec3 outward_normal)
    {
        front_face = dot(ray->direction, outward_normal) < 0.0f;
        normal = front_face ? outward_normal : -outward_normal;
    }
};
