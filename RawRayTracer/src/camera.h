#pragma once
#include "ray.h"

class camera
{
public:
    camera()
    {
        lower_left = vec3(-2.0f, -1.0f, -1.0f);
        horizontal = vec3(4.0f, 0.0f, 0.0f);
        vertical = vec3(0.0f, 2.0f, 0.0f);
        origin = vec3(0.0f, 0.0f, 0.0f);
    }
    inline ray get_ray(float u, float v) const { return ray(origin, lower_left + u * horizontal + v * vertical - origin); }
    vec3 origin;
    vec3 lower_left;
    vec3 horizontal;
    vec3 vertical;
};