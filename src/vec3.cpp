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


float length(Vec3 v)
{
    float res = sqrtf(length_squared(v));

    return res;
}

float length_squared(Vec3 v)
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

Vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

Vec3 random_in_hemisphere(Vec3 normal)
{
    Vec3 in_unit_sphere = random_in_unit_sphere();

    if (dot(in_unit_sphere, normal) > 0.0f) // in the hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

bool near_zero(Vec3 v)
{
    // Return true if the vector is close to zero in all dimensions.
    float s = 1e-8f;
    return 
        (fabsf(v.x) < s) && 
        (fabsf(v.y) < s) && 
        (fabsf(v.z) < s);
}

Vec3 reflect(Vec3 v, Vec3 n)
{
    return v - 2.0f * dot(v, n) * n;
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

Vec3 unit_vector(Vec3 a)
{
    return a / length(a);
}

Vec3 random_in_unit_sphere()
{
    while (true)
    {
        Vec3 p = random_vec3(-1.0f, 1.0f);

        if (length_squared(p) >= 1.0f)
            continue;

        return p;
    }
}

Vec3 refract(Vec3 uv, Vec3 n, float etai_over_etat)
{
    float cos_theta = fminf(dot(-uv, n), 1.0f);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel = -sqrtf(fabsf(1.0f - length_squared(r_out_perp))) * n;
    return r_out_perp + r_out_parallel;
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

