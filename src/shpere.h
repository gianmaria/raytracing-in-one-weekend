#pragma once

#include "hittable.h"
#include "vec3.h"

struct Material;

struct Sphere
{
    Point3 center;
    float radius;
    Material* material;
};

Sphere sphere(Point3 center, float radius, Material* material);

bool ray_hit_sphere(const Sphere* sphere, 
    const Ray* ray, float t_min, float t_max, Hit_Record* rec);
