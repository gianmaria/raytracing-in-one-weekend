#pragma once

#include "hittable.h"
#include "vec3.h"

struct Sphere
{
    Point3 center;
    float radius;

};

Sphere sphere(Point3 center, float radius);

bool ray_hit_sphere(Sphere* sphere, 
    Ray* ray, float t_min, float t_max, Hit_Record* rec);
