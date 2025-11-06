#pragma once
#include <random>
#include "vec3.h"

float randnum()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0.0f, 1.0f);
    return dis(gen);
}

class ray
{
public:
    ray() {}
    ray(const vec3 &origin, const vec3 &direction, float time = 0.0f)
    {
        m_origin = origin;
        m_direction_vector = direction;
        m_time = time;
    }
    inline vec3 origin() const { return m_origin; }
    inline vec3 direction() const { return m_direction_vector; }
    inline float time() const { return m_time; }
    inline vec3 point_at_parameter(float t) const { return m_origin + t * m_direction_vector; } // Ray equation: ray = A + t*B
private:
    vec3 m_origin;
    vec3 m_direction_vector;
    float m_time;
};