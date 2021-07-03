#include <math.h>

#include "vec3.h"

Vec3 vec3()
{
    Vec3 res = {};

    res.x = 0.0f;
    res.y = 0.0f;
    res.z = 0.0f;

    return res;
}

Vec3 vec3(float val)
{
    Vec3 res = {};

    res.x = val;
    res.y = val;
    res.z = val;

    return res;
}

Vec3 vec3(float a, float b, float c)
{
    Vec3 res = {};

    res.x = a;
    res.y = b;
    res.z = c;

    return res;
}


float len(const Vec3& a)
{
    float res = sqrtf(
        a.x * a.x +
        a.y * a.y +
        a.z * a.z);

    return res;
}

float dot(const Vec3& a, const Vec3& b)
{
    float res =
        a.x * b.x +
        a.y * b.y +
        a.z * b.z;

    return res;
}

Vec3 cross(const Vec3& a, const Vec3& b)
{
    Vec3 res = {};

    res.x = a.y * b.z - a.z * b.y;
    res.y = a.z * b.x - a.x * b.z;
    res.z = a.x * b.y - a.y * b.x;

    return res;
}

Vec3 unit(const Vec3& a)
{
    return a / len(a);
}


Vec3 operator+(const Vec3& a, const Vec3& b)
{
    Vec3 res = {};

    res.x = a.x + b.x;
    res.y = a.y + b.y;
    res.z = a.z + b.z;

    return res;
}

Vec3 operator-(const Vec3& a, const Vec3& b)
{
    Vec3 res = {};

    res.x = a.x - b.x;
    res.y = a.y - b.y;
    res.z = a.z - b.z;

    return res;
}

Vec3 operator*(const Vec3& a, const Vec3& b)
{
    Vec3 res = {};

    res.x = a.x * b.x;
    res.y = a.y * b.y;
    res.z = a.z * b.z;

    return res;
}

Vec3 operator*(const Vec3& a, float val)
{
    Vec3 res = {};

    res.x = a.x * val;
    res.y = a.y * val;
    res.z = a.z * val;

    return res;
}

Vec3 operator*(float val, const Vec3& a)
{
    return a * val;
}

Vec3 operator/(const Vec3& a, float val)
{
    Vec3 res = {};

    res.x = a.x / val;
    res.y = a.y / val;
    res.z = a.z / val;

    return res;
}

Vec3 operator-(const Vec3& a)
{
    Vec3 res = {};

    res.x = -a.x;
    res.y = -a.y;
    res.z = -a.z;

    return res;
}

Vec3& operator+=(Vec3& a, Vec3 b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;

    return a;
}

Vec3& operator*=(Vec3& a, float val)
{
    a.x *= val;
    a.y *= val;
    a.z *= val;

    return a;
}

Vec3& operator/=(Vec3& a, float val)
{
    a.x /= val;
    a.y /= val;
    a.z /= val;

    return a;
}

bool operator==(const Vec3& a, const Vec3& b)
{
    bool res =
        a.x == b.x &&
        a.y == b.y &&
        a.z == b.z;

    return res;
}

