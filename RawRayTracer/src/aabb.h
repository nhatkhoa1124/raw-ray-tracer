#pragma once
#include "ray.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class aabb
{
public:
    aabb() {}
    aabb(const vec3 &min, const vec3 &max) { m_min = min, m_max = max; }
    inline vec3 min() const { return m_min; }
    inline vec3 max() const { return m_max; }
    bool hit(const ray &r, float tmin, float tmax) const;

    vec3 m_min;
    vec3 m_max;
};

inline bool aabb::hit(const ray &ray, float rayStart, float rayEnd) const
{
    for (int dimension = 0; dimension < 3; dimension++)
    {
        float invD = 1.0f / ray.direction()[dimension];
        float enterSlab = (min()[dimension] - ray.origin()[dimension]) * invD;
        float leaveSlab = (max()[dimension] - ray.origin()[dimension]) * invD;
        if (invD < 0.0f)
            std::swap(enterSlab, leaveSlab);
        rayStart = ffmax(enterSlab, rayStart);
        rayEnd = ffmin(leaveSlab, rayEnd);
        if (rayEnd <= rayStart)
        {
            return false;
        }
    }
    return true;
}

// Calculate bounding boxes for a moving object
inline aabb surrounding_box(aabb box0, aabb box1)
{
    vec3 small(ffmin(box0.min().x(), box1.min().x()),
               ffmin(box0.min().y(), box1.min().y()),
               ffmin(box0.min().z(), box1.min().z()));
    vec3 big(ffmax(box0.max().x(), box1.max().x()),
             ffmax(box0.max().y(), box1.max().y()),
             ffmax(box0.max().z(), box1.max().z()));
    return aabb(small, big);
}