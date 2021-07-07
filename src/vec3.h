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
Vec3 vec3(float x, float y, float z);

Color color();
Color color(float val);
Color color(float r, float g, float b);

float length(Vec3 v);
float length_squared(Vec3 v);

Vec3 random_vec3();
Vec3 random_vec3(float min, float max);
Vec3 random_unit_vector();
Vec3 random_in_hemisphere(Vec3 normal);
Vec3 random_in_unit_disk();
bool near_zero(Vec3 v);
Vec3 reflect(Vec3 v, Vec3 n);

float dot(Vec3 a, Vec3 b);
Vec3 cross(Vec3 a, Vec3 b);
Vec3 unit_vector(Vec3 a);

Vec3 random_in_unit_sphere();

Vec3 refract(Vec3 uv, Vec3 n, float etai_over_etat);




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