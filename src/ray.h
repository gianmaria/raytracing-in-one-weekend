#pragma once

#include "vec3.h"

struct Ray
{    
    Point3 origin;
    Vec3 direction;

    Point3 at(float t);
};

Ray ray(Point3 origin, Vec3 dir);
