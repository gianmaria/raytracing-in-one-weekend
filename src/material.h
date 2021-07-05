#pragma once

#include "vec3.h"

struct Hit_Record;
struct Ray;

enum class Material_Type : unsigned
{
    lambertian, metal, dielectric
};

struct Lambertian_Material
{
    Color color;
};

struct Metal_Material
{
    Color color;
    float fuzz;
};

struct Dielectric_Material
{
    float ir; // Index of Refraction
};

struct Material
{
    Material_Type type;

    union
    {
        Lambertian_Material lambertian;
        Metal_Material metal;
        Dielectric_Material dielectric;
    };

};


bool scatter(
    const Material* material,
    const Ray* r_in, const Hit_Record* rec,
    Color* attenuation, Ray* scattered);
