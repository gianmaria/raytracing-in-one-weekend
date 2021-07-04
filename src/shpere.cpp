#include <math.h>

#include "shpere.h"

bool ray_hit_sphere(Sphere* sphere,
    Ray* ray, float t_min, float t_max, Hit_Record* rec)
{
    Vec3 oc = ray->origin - sphere->center;
    float a = len_squared(ray->direction);
    float half_b = dot(oc, ray->direction);
    float c = len_squared(oc) - sphere->radius * sphere->radius;
    float discriminant = half_b * half_b - a * c;

    if (discriminant < 0.0f)
        return false;
    
    float sqrtd = sqrtf(discriminant);

    // find the nearest root that lies in the accetable range.
    float root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) 
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec->t = root;
    rec->point = ray->at(rec->t);
    rec->normal = (rec->point - sphere->center) / sphere->radius;
    Vec3 outward_normal = (rec->point - sphere->center) / sphere->radius;
    rec->set_face_normal(ray, outward_normal);

    return true;
}

Sphere sphere(Point3 center, float radius)
{
    Sphere res = {};

    res.center = center;
    res.radius = radius;

    return res;
}
