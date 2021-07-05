#pragma once

#include "vec3.h"

struct Hit_Record;
struct Ray;

enum class Material_Type : unsigned
{
    lambertian, metal, dielectric
};

struct Material
{
    Material_Type type;
    Color color;
    float fuzz;
    double ir; // Index of Refraction
};

Material material(Color color, Material_Type type, 
    float fuzz, float index_of_refraction);

bool lambertian_scatter(
    Material* material,
    Ray* r_in, Hit_Record* rec,
    Color* attenuation, Ray* scattered);

bool metal_scatter(
    Material* material,
    Ray* r_in, Hit_Record* rec,
    Color* attenuation, Ray* scattered);

bool dielectric_scatter(Material* material,
    Ray* r_in, Hit_Record* rec,
    Color* attenuation, Ray* scattered);
