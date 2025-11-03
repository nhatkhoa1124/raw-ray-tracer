#pragma once
#include "vec3.h"

class ray
{
public:
    ray(){}
    ray(const vec3& o, const vec3& d) {origin_point = o; direction_vector = d;}
    vec3 origin() const {return origin_point;}
    vec3 direction() const {return direction_vector;}
    vec3 point_at_parameter(float t) const {return origin_point + t*direction_vector;} // Ray equation: ray = A + t*B
private:
    vec3 origin_point; 
    vec3 direction_vector;
};