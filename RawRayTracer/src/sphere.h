#pragma once
#include "hitable.h"

class sphere : public hitable
{
public:
    sphere() {}
    sphere(const vec3 &center, float radius, material *pmat) : center{center}, radius{radius}, m_mat_ptr{pmat} {}
    bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const override;
    vec3 center;
    float radius;
    material *m_mat_ptr;
};

bool sphere::hit(const ray &r, float tmin, float tmax, hit_record &rec) const
{
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp > tmin && temp < tmax)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = m_mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp > tmin && temp < tmax)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = m_mat_ptr;
            return true;
        }
    }
    return false;
}

class moving_sphere : public hitable
{
public:
    moving_sphere() {}
    moving_sphere(vec3 cen0, vec3 cen1, float time0, float time1, float r, material *m) : m_center0{cen0},
                                                                                          m_center1{cen1},
                                                                                          m_time0{time0},
                                                                                          m_time1{time1},
                                                                                          m_radius{r},
                                                                                          m_mat_ptr{m}
    {
    }
    bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const override;
    vec3 center(float time) const;

private:
    vec3 m_center0, m_center1;
    float m_time0, m_time1;
    float m_radius;
    material *m_mat_ptr;
};

bool moving_sphere::hit(const ray &r, float tmin, float tmax, hit_record &rec) const
{
    vec3 oc = r.origin() - center(r.time());
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - m_radius * m_radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp > tmin && temp < tmax)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / m_radius;
            rec.mat_ptr = m_mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp > tmin && temp < tmax)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / m_radius;
            rec.mat_ptr = m_mat_ptr;
            return true;
        }
    }
    return false;
}

vec3 moving_sphere::center(float time) const
{
    return m_center0 + ((time - m_time0) / (m_time1 - m_time0)) * (m_center1 - m_center0);
}