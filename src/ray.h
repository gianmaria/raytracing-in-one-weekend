#pragma once

#include "vec3.h"

struct Ray
{    
    Point3 origin;
    Vec3 direction;
};

Ray ray(Point3 origin, Vec3 dir);

Point3 ray_at(const Ray* ray, float t);