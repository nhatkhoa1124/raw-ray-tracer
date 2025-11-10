#pragma once
#include "hitable.h"
#include <algorithm>  // Use std::sort instead of qsort
#include <iostream>

class bvh_node : public hitable 
{
public:
    bvh_node() : m_left(nullptr), m_right(nullptr) {}
    bvh_node(hitable **l, int n, float time0, float time1);
    virtual ~bvh_node();  // Add destructor for cleanup
    
    bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const override;
    bool bounding_box(float time0, float time1, aabb &box) const override;
    
private:
    hitable *m_left;
    hitable *m_right;
    aabb m_box;

    // Comparison functions as static members
    static bool box_compare_x(hitable *a, hitable *b);
    static bool box_compare_y(hitable *a, hitable *b); 
    static bool box_compare_z(hitable *a, hitable *b);
};

// Implementation
inline bool bvh_node::box_compare_x(hitable *a, hitable *b) 
{
    aabb box_a, box_b;
    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b)) {
        std::cerr << "No bounding box in bvh_node constructor\n";
        return false;
    }
    return box_a.min().x() < box_b.min().x();
}

inline bool bvh_node::box_compare_y(hitable *a, hitable *b) 
{
    aabb box_a, box_b;
    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b)) {
        std::cerr << "No bounding box in bvh_node constructor\n";
        return false;
    }
    return box_a.min().y() < box_b.min().y();
}

inline bool bvh_node::box_compare_z(hitable *a, hitable *b) 
{
    aabb box_a, box_b;
    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b)) {
        std::cerr << "No bounding box in bvh_node constructor\n";
        return false;
    }
    return box_a.min().z() < box_b.min().z();
}

inline bvh_node::bvh_node(hitable **l, int n, float time0, float time1) 
    : m_left(nullptr), m_right(nullptr)
{
    int axis = int(3 * randnum());
    
    // Use std::sort instead of qsort
    if (axis == 0) {
        std::sort(l, l + n, box_compare_x);
    } else if (axis == 1) {
        std::sort(l, l + n, box_compare_y);
    } else {
        std::sort(l, l + n, box_compare_z);
    }

    if (n == 1) {
        m_left = m_right = l[0];
    } else if (n == 2) {
        m_left = l[0];
        m_right = l[1];
    } else {
        int mid = n / 2;
        m_left = new bvh_node(l, mid, time0, time1);
        m_right = new bvh_node(l + mid, n - mid, time0, time1);
    }
    
    aabb box_left, box_right;
    if (!m_left->bounding_box(time0, time1, box_left) || 
        !m_right->bounding_box(time0, time1, box_right)) {
        std::cerr << "No bounding box in bvh_node constructor\n";
    }
    m_box = surrounding_box(box_left, box_right);
}

inline bvh_node::~bvh_node() {
    // Clean up only the BVH nodes we created, not the original objects
    if (m_left != m_right) {  // If they're different, we allocated them
        if (m_left && dynamic_cast<bvh_node*>(m_left)) delete m_left;
        if (m_right && dynamic_cast<bvh_node*>(m_right)) delete m_right;
    }
}

inline bool bvh_node::hit(const ray &r, float tmin, float tmax, hit_record &rec) const {
    if (!m_box.hit(r, tmin, tmax)) {
        return false;
    }
    
    hit_record left_rec, right_rec;
    bool hit_left = m_left->hit(r, tmin, tmax, left_rec);
    bool hit_right = m_right->hit(r, tmin, tmax, right_rec);
    
    if (hit_left && hit_right) {
        rec = (left_rec.t < right_rec.t) ? left_rec : right_rec;
        return true;
    } else if (hit_left) {
        rec = left_rec;
        return true;
    } else if (hit_right) {
        rec = right_rec;
        return true;
    }
    return false;
}

inline bool bvh_node::bounding_box(float time0, float time1, aabb &box) const {
    box = m_box;
    return true;
}