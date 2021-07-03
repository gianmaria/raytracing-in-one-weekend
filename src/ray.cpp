#include "ray.h"


Ray::Ray(const Point3& origin, const Vec3& dir) :
    origin(origin), direction(dir)
{
}

Point3 Ray::at(float t)
{
    return origin + t * direction;
}