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


float dot(Vec3 a, Vec3 b);
Vec3 cross(Vec3 a, Vec3 b);
Vec3 unit(Vec3 a);
float len(Vec3 a);

Vec3 operator-(Vec3 a);

Vec3 operator+(Vec3 a, Vec3 b);
Vec3 operator-(Vec3 a, Vec3 b);
Vec3 operator*(Vec3 a, Vec3 b);
Vec3 operator*(Vec3 a, float val);
Vec3 operator*(float val, Vec3 a);
Vec3 operator/(Vec3 a, float val);

Vec3& operator+=(Vec3& a, Vec3 b);
Vec3& operator*=(Vec3& a, float val);
Vec3& operator/=(Vec3& a, float val);

bool operator==(Vec3 a, Vec3 b);