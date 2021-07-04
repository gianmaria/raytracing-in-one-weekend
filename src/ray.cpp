#include "ray.h"


Ray ray(Point3 origin, Vec3 dir)
{
    Ray res = {};

    res.origin = origin;
    res.direction = dir;

    return res;
}

Point3 Ray::at(float t)
{
    return origin + t * direction;
}