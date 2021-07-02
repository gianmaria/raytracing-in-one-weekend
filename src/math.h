#pragma once

union Vec3
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

using Color = Vec3;
using Point3 = Vec3;

Vec3 operator+(const Vec3& a, const Vec3& b);