#include "ray.h"


Ray ray(Point3 origin, Vec3 dir)
{
    Ray res = {};

    res.origin = origin;
    res.direction = dir;

    return res;
}

Point3 ray_at(Ray* ray, float t)
{
    return ray->origin + t * ray->direction;
}