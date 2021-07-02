#include "math.h"

Vec3 operator+(const Vec3& a, const Vec3& b)
{
    Vec3 res = {};

    res.x = a.x + b.x;
    res.y = a.y + b.y;
    res.z = a.z + b.z;

    return res;
}
