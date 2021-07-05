#include "material.h"
#include "ray.h"
#include "hittable.h"
#include "vec3.h"



static bool lambertian_scatter(
    const Lambertian_Material* material,
    const Ray* r_in, const Hit_Record* rec, 
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

static bool metal_scatter(
    const Metal_Material* material,
    const Ray* r_in, const Hit_Record* rec,
    Color* attenuation, Ray* scattered)
{
    Vec3 reflected = reflect(unit_vector(r_in->direction), rec->normal);
    *scattered = ray(rec->point, reflected + material->fuzz * random_in_unit_sphere());
    *attenuation = material->color;

    return (dot(scattered->direction, rec->normal) > 0.0f);
}

static bool dielectric_scatter(
    const Dielectric_Material* material,
    const Ray* r_in, const Hit_Record* rec, 
    Color* attenuation, Ray* scattered)
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


bool scatter(
    const Material* material,
    const Ray* r_in, const Hit_Record* rec,
    Color* attenuation, Ray* scattered)
{
    switch (material->type)
    {
    case Material_Type::lambertian:
        return lambertian_scatter(
            &material->lambertian,
            r_in, rec,
            attenuation, scattered);
        break;

    case Material_Type::metal:
        return metal_scatter(
            &material->metal,
            r_in, rec,
            attenuation, scattered);
        break;

    case Material_Type::dielectric:
        return dielectric_scatter(
            &material->dielectric,
            r_in, rec,
            attenuation, scattered
        );
        break;
    }

    return false;
}