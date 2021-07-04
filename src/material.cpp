#include "material.h"
#include "ray.h"
#include "hittable.h"
#include "vec3.h"

Material material(Color color, Material_Type type)
{
    Material material = {};

    material.color = color;
    material.type = type;

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
    *scattered = ray(rec->point, reflected);
    *attenuation = material->color;

    return (dot(scattered->direction, rec->normal) > 0.0f);
}
