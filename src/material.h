#pragma once

#include "vec3.h"

struct Hit_Record;
struct Ray;

enum class Material_Type : unsigned
{
    lambertian, metal
};

struct Material
{
    Color color;
    Material_Type type;
};

Material material(Color color, Material_Type type);

bool lambertian_scatter(
    Material* material,
    Ray* r_in, Hit_Record* rec,
    Color* attenuation, Ray* scattered);

bool metal_scatter(
    Material* material,
    Ray* r_in, Hit_Record* rec,
    Color* attenuation, Ray* scattered);
