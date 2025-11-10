#pragma once
#include <random>
#include "hitable.h"
#include "texture.h"

vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0f * vec3(randnum(), randnum(), randnum()) - vec3(1.0f, 1.0f, 1.0f);
    } while (p.squared_length() >= 1.0f);
    return p;
}

vec3 reflect(const vec3 &in_light, const vec3 &normal)
{
    return in_light - 2 * dot(in_light, normal) * normal;
}

bool refract(const vec3 &v_in, const vec3 &normal, float ni_over_nt, vec3 &refracted)
{
    vec3 uv = unit_vector(v_in);
    float cos_theta1 = dot(uv, normal);                                               // equals to cosine of the angle between vector in and surface normal
    float sin_theta2_sq = ni_over_nt * ni_over_nt * (1.0f - cos_theta1 * cos_theta1); // sin = ni/nt*(sin_theta1) = ni/nt*(1-cos_theta1)
    float cos_theta2_sq = 1.0f - sin_theta2_sq;                                       // checks if sine is smaller or bigger than 1
    if (cos_theta2_sq > 0)                                                            // if sine is smaller (1-sin>0) than 1, then refraction is possible
    {
        refracted = ni_over_nt * (uv - normal * cos_theta1) - normal * sqrt(cos_theta2_sq);
        return true;
    }
    return false;
}

float schlick(float cosine, float ref_idx)
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class material
{
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(texture *tex) : m_albedo{tex} {}
    inline bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p, r_in.time());
        attenuation = m_albedo->value(0.0f, 0.0f, rec.p);
        return true;
    }
    texture *m_albedo;
};

class metal : public material
{
public:
    metal(const vec3 &a, float f) : albedo{a}, fuzz{f} {}
    inline bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override
    {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    vec3 albedo;
    float fuzz;
};

class dielectric : public material
{
public:
    dielectric(float ri) : ref_idx{ri} {};
    inline bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override
    {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0f, 1.0f, 1.0f);
        vec3 refracted;
        float reflect_prob;
        float cosine;
        if (dot(r_in.direction(), rec.normal) > 0) // ray hitting from INSIDE
        {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        else // ray hitting from OUTSIDE
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.0f / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
        {
            reflect_prob = schlick(cosine, ref_idx);
        }
        else
        {
            scattered = ray(rec.p, reflected);
            reflect_prob = 1.0f;
        }
        if (randnum() < reflect_prob)
        {
            scattered = ray(rec.p, reflected);
        }
        else
        {
            scattered = ray(rec.p, refracted);
        }
        return true;
    }
    float ref_idx;
};