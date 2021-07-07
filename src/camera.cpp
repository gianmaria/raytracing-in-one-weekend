#include "camera.h"
#include "rtweekend.h"

Camera camera(
    Point3 lookfrom,
    Point3 lookat,
    Vec3 vup,
    float vfov, // vertical field-of-view in degrees
    float aspect_ratio)
{
    Camera camera = {};

    float theta = degrees_to_radians(vfov);
    float h = tanf(theta / 2.0f);
    float viewport_height = 2.0f * h;
    float viewport_width = aspect_ratio * viewport_height;

    Vec3 w = unit_vector(lookfrom - lookat);
    Vec3 u = unit_vector(cross(vup, w));
    Vec3 v = cross(w, u);

    camera.origin = lookfrom;
    camera.horizontal = viewport_width * u;
    camera.vertical = viewport_height * v;
    camera.lower_left_corner =
        camera.origin -
        camera.horizontal / 2.0f - camera.vertical / 2.0f -
        w;

    return camera;
}

Ray get_ray_from_camera(Camera* camera, float s, float t)
{
    Ray ray = {};

    ray.origin = camera->origin;

    ray.direction =
        camera->lower_left_corner +
        s * camera->horizontal +
        t * camera->vertical - camera->origin;

    return ray;
}
