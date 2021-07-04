#include "camera.h"

Camera default_camera()
{
    Camera camera;

    float aspect_ratio = 16.0f / 9.0f;
    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height;
    float focal_length = 1.0f;

    camera.origin = vec3(0.0f);
    camera.horizontal = vec3(viewport_width, 0.0f, 0.0f);
    camera.vertical = vec3(0.0f, viewport_height, 0.0f);
    camera.lower_left_corner = 
        camera.origin - 
        camera.horizontal / 2.0f - camera.vertical / 2.0f - 
        vec3(0.0f, 0.0f, focal_length);

    return camera;
}

Ray get_ray_from_camera(Camera* camera, float u, float v)
{
    Ray ray = {};

    ray.origin = camera->origin;

    ray.direction =
        camera->lower_left_corner + u *
        camera->horizontal + v *
        camera->vertical - camera->origin;

    return ray;
}
