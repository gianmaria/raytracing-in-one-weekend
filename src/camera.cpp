#include "camera.h"
#include "rtweekend.h"

Camera camera(
    Point3 lookfrom,
    Point3 lookat,
    Vec3 vup,
    float vfov, // vertical field-of-view in degrees
    float aspect_ratio,
    float aperture,
    float focus_distance)
{
    Camera camera = {};

    float theta = degrees_to_radians(vfov);
    float h = tanf(theta / 2.0f);
    float viewport_height = 2.0f * h;
    float viewport_width = aspect_ratio * viewport_height;

    camera.w = unit_vector(lookfrom - lookat);
    camera.u = unit_vector(cross(vup, camera.w));
    camera.v = cross(camera.w, camera.u);

    camera.origin = lookfrom;
    camera.horizontal = focus_distance * viewport_width * camera.u;
    camera.vertical = focus_distance * viewport_height * camera.v;
    camera.lower_left_corner =
        (camera.origin -
        camera.horizontal / 2.0f -
        camera.vertical / 2.0f -
        focus_distance * camera.w);
    camera.lens_radius = aperture / 2.0f;

    return camera;
}

Ray get_ray_from_camera(const Camera* camera, float s, float t)
{
    Ray ray = {};

    Vec3 rd = camera->lens_radius * random_in_unit_disk();
    Vec3 offset = camera->u * rd.x + camera->v * rd.y;

    ray.origin = camera->origin + offset;

    ray.direction =
        camera->lower_left_corner +
        s * camera->horizontal +
        t * camera->vertical - 
        camera->origin - offset;

    return ray;
}
