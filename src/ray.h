#pragma once

#include "vec3.h"

struct Ray
{
    Ray(const Point3& origin, const Vec3& dir);

    Point3 at(float t);
    
    Point3 origin;
    Vec3 direction;
};
