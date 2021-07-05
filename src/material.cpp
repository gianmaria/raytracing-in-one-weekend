#include "material.h"
#include "ray.h"
#include "hittable.h"
#include "vec3.h"

Material material(
    Color color, Material_Type type, 
    float fuzz, 
    float index_of_refraction)
{
    Material material = {};

    material.color = color;
    material.type = type;
    material.fuzz = fuzz;
    material.ir = index_of_refraction;

    return material;
}

bool lambertian_scatter(
    Material* material,
    Ray* r_in, Hit_Record* rec, 
    Color* attenuation, Ray* scattered)
{
    (void)r_in;

    Vec3 scatter_direction = rec->normal + random_unit_vector();

    // catch degenerate scatter direction
    if (near_zero(scatter_direction))
        scatter_direction = rec->normal;

    *scattered = ray(rec->point, scatter_direction);
    *attenuation = material->color;
    
    return true;
}

bool metal_scatter(
    Material* material, 
    Ray* r_in, Hit_Record* rec, 
    Color* attenuation, Ray* scattered)
{
    Vec3 reflected = reflect(unit_vector(r_in->direction), rec->normal);
    *scattered = ray(rec->point, reflected + material->fuzz * random_in_unit_sphere());
    *attenuation = material->color;

    return (dot(scattered->direction, rec->normal) > 0.0f);
}

bool dielectric_scatter(
    Material* material, 
    Ray* r_in, 
    Hit_Record* rec, 
    Color* attenuation, 
    Ray* scattered)
{
    *attenuation = vec3(1.0f, 1.0f, 1.0f);
    float refraction_ratio = rec->front_face ? 
        (1.0f / material->ir) : 
        material->ir;

    Vec3 unit_direction = unit_vector(r_in->direction);
    Vec3 refracted = refract(unit_direction, rec->normal, refraction_ratio);

    *scattered = ray(rec->point, refracted);

    return true;
}
