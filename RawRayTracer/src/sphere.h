#pragma once
#include "hitable.h"

class sphere : public hitable
{
public:
    sphere(){}
    sphere(const vec3& center, float radius, material* pmat): center{center}, radius{radius}, p_material{pmat} {}
    bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const override;
    vec3 center;
    float radius;
    material* p_material;
};

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const 
{
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    if(discriminant > 0)
    {
        float temp = (-b - sqrt(b*b-a*c))/a;
        if(temp > tmin && temp < tmax)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center)/radius;
            rec.mat_ptr = p_material;
            return true;
        }
        temp = (-b + sqrt(b*b - a*c))/a;
        if(temp > tmin && temp < tmax)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center)/radius;
            rec.mat_ptr = p_material;
            return true;
        }
    }
    return false;
}