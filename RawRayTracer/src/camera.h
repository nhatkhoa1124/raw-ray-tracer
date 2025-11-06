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
    camera(const vec3 &look_from, const vec3 &look_at, const vec3 &vup, float vfov, float aspect_ratio, float aperture, float focus_dist,
           float time0, float time1)
    {
        m_time0 = time0;
        m_time1 = time1;
        m_lens_radius = aperture / 2;
        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect_ratio * half_height;
        m_origin = look_from;
        m_w = unit_vector(look_from - look_at);
        m_u = unit_vector(cross(vup, m_w));
        m_v = unit_vector(cross(m_w, m_u));
        m_lower_left = m_origin - half_width * focus_dist * m_u - half_height * focus_dist * m_v - m_w * focus_dist;
        m_horizontal = 2 * half_width * focus_dist * m_u;
        m_vertical = 2 * half_height * focus_dist * m_v;
    }
    inline ray get_ray(float s, float t) const
    {
        vec3 rd = m_lens_radius * random_in_unit_disk(); // scales from unit disk to lens radius
        vec3 offset = m_u * rd.x() + m_v * rd.y();       // convert from lens space to camera's uv space
        float time = m_time0 + randnum() * (m_time1 - m_time0);
        return ray(m_origin + offset, m_lower_left + s * m_horizontal + t * m_vertical - m_origin - offset, time);
    }
    vec3 m_origin;
    vec3 m_lower_left;
    vec3 m_horizontal;
    vec3 m_vertical;
    vec3 m_u, m_v, m_w; // camera's xyz vector
    float m_time0, m_time1;
    float m_lens_radius;
};