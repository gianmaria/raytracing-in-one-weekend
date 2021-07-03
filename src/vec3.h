#pragma once

struct Vec3
{
    union
    {
        struct
        {
            float x, y, z;
        };

        struct
        {
            float r, g, b;
        };

        float e[3];
    };
};

using Color = Vec3;
using Point3 = Vec3;

Vec3 vec3();
Vec3 vec3(float val);
Vec3 vec3(float a, float b, float c);


float dot(const Vec3& a, const Vec3& b);
Vec3 cross(const Vec3& a, const Vec3& b);
Vec3 unit(const Vec3& a);
float len(const Vec3& a);

Vec3 operator-(const Vec3& a);

Vec3 operator+(const Vec3& a, const Vec3& b);
Vec3 operator-(const Vec3& a, const Vec3& b);
Vec3 operator*(const Vec3& a, const Vec3& b);
Vec3 operator*(const Vec3& a, float val);
Vec3 operator*(float val, const Vec3& a);
Vec3 operator/(const Vec3& a, float val);

Vec3& operator+=(Vec3& a, Vec3 b);
Vec3& operator*=(Vec3& a, float val);
Vec3& operator/=(Vec3& a, float val);

bool operator==(const Vec3& a, const Vec3& b);