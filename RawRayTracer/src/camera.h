#pragma once
#include "ray.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

vec3 random_in_unit_disk()
{
    vec3 p;
    do
    {
        p = 2.0f * vec3(randnum(), randnum(), 0.0f) - vec3(1.0f, 1.0f, 0.0f);
    } while (dot(p, p) >= 1.0f);
    return p;
}

class camera
{
public:
    camera(const vec3 &look_from, const vec3 &look_at, const vec3 &vup, float vfov, float aspect_ratio, float aperture, float focus_dist)
    {
        lens_radius = aperture / 2;
        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect_ratio * half_height;
        origin = look_from;
        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(vup, w));
        v = unit_vector(cross(w, u));
        lower_left = origin - half_width * focus_dist * u - half_height * focus_dist * v - w * focus_dist;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }
    inline ray get_ray(float s, float t) const
    {
        vec3 rd = lens_radius * random_in_unit_disk(); // scales from unit disk to lens radius
        vec3 offset = u * rd.x() + v * rd.y();         // convert from lens space to camera's uv space
        return ray(origin + offset, lower_left + s * horizontal + t * vertical - origin - offset);
    }
    vec3 origin;
    vec3 lower_left;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};