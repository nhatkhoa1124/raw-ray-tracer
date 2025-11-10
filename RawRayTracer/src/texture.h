#pragma once
#include "vec3.h"

class texture
{
public:
    virtual vec3 value(float u, float v, const vec3 &p) const = 0;
};

class constant_texture : public texture
{
public:
    constant_texture() {};
    constant_texture(vec3 color) : m_color{color} {}
    inline vec3 value(float u, float v, const vec3 &p) const override
    {
        // returns a constant color
        return m_color;
    }
    vec3 m_color;
};

class checker_texture : public texture
{
public:
    checker_texture() {}
    checker_texture(texture *tex0, texture *tex1) : odd{tex0}, even{tex1} {}
    inline vec3 value(float u, float v, const vec3 &p) const override
    {
        float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }
    texture *odd;
    texture *even;
};