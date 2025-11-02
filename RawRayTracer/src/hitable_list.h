#pragma once
#include "hitable.h"

class hitable_list : public hitable
{
public:
    hitable_list() {}
    hitable_list(hitable **l, int n) : list{l}, list_size{n} {}
    bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const override;
    hitable **list;
    int list_size;
};

bool hitable_list::hit(const ray& r, float tmin, float tmax, hit_record& rec) const
{
    hit_record temp_rec;
    float closest_hit_point = tmax;
    bool hit_anything = false;
    for(int i = 0; i < list_size; i++)
    {
        if(list[i]->hit(r, tmin, closest_hit_point, temp_rec))
        {
            hit_anything = true;
            closest_hit_point = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}