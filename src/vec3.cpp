#include <math.h>

#include "vec3.h"
#include "rtweekend.h"

Vec3 vec3()
{
    Vec3 res = {};

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


float len(Vec3 v)
{
    float res = sqrtf(len_squared(v));

    return res;
}

float len_squared(Vec3 v)
{
    float res = 
        v.x * v.x +
        v.y * v.y +
        v.z * v.z;

    return res;
}


Vec3 random_vec3()
{
    return vec3(
        random_float(),
        random_float(),
        random_float());
}

Vec3 random_vec3(float min, float max)
{
    return vec3(
        random_float(min, max),
        random_float(min, max),
        random_float(min, max));
}

float dot(Vec3 a, Vec3 b)
{
    float res =
        a.x * b.x +
        a.y * b.y +
        a.z * b.z;

    return res;
}

Vec3 cross(Vec3 a, Vec3 b)
{
    Vec3 res = {};

    res.x = a.y * b.z - a.z * b.y;
    res.y = a.z * b.x - a.x * b.z;
    res.z = a.x * b.y - a.y * b.x;

    return res;
}

Vec3 unit_vec(Vec3 a)
{
    return a / len(a);
}

Vec3 random_in_unit_sphere()
{
    while (true)
    {
        Vec3 p = random_vec3(-1.0f, 1.0f);

        if (len_squared(p) >= 1.0f)
            continue;

        return p;
    }
}


Vec3 operator+(Vec3 a, Vec3 b)
{
    Vec3 res = {};

    res.x = a.x + b.x;
    res.y = a.y + b.y;
    res.z = a.z + b.z;

    return res;
}

Vec3 operator-(Vec3 a, Vec3 b)
{
    Vec3 res = {};

    res.x = a.x - b.x;
    res.y = a.y - b.y;
    res.z = a.z - b.z;

    return res;
}

Vec3 operator*(Vec3 a, Vec3 b)
{
    Vec3 res = {};

    res.x = a.x * b.x;
    res.y = a.y * b.y;
    res.z = a.z * b.z;

    return res;
}

Vec3 operator*(Vec3 a, float val)
{
    Vec3 res = {};

    res.x = a.x * val;
    res.y = a.y * val;
    res.z = a.z * val;

    return res;
}

Vec3 operator*(float val, Vec3 a)
{
    return a * val;
}

Vec3 operator/(Vec3 a, float val)
{
    Vec3 res = {};

    res.x = a.x / val;
    res.y = a.y / val;
    res.z = a.z / val;

    return res;
}

Vec3 operator-(Vec3 a)
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

bool operator==(Vec3 a, Vec3 b)
{
    bool res =
        a.x == b.x &&
        a.y == b.y &&
        a.z == b.z;

    return res;
}

