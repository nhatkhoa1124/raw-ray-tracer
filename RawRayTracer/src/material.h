#pragma once
#include <random>
#include "hitable.h"

float randnum()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0.0f, 1.0f);
    return dis(gen);
}

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

class material
{
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const vec3 &a) : albedo{a} {}
    inline bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }
    vec3 albedo;
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