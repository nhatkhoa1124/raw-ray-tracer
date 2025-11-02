#include <fstream>
#include "sphere.h"
#include "hitable_list.h"

#define TMAX 10000.0f

vec3 color(const ray &r, hitable *world)
{
    hit_record rec;
    if (world->hit(r, 0.1f, TMAX, rec))
    {
        return 0.5 * vec3(rec.normal.x() + 1.0f, rec.normal.y() + 1.0f, rec.normal.z() + 1.0f);
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
    }
}

int main()
{
    int width = 800, height = 400;
    std::ofstream file("images\\output.ppm");
    file << "P3\n"
         << width << " " << height << "\n255\n";
    vec3 lower_left(-2.0f, -1.0f, -1.0f);
    vec3 horizontal(4.0f, 0.0f, 0.0f);
    vec3 vertical(0.0f, 2.0f, 0.0f);
    vec3 origin(0.0f, 0.0f, 0.0f);
    // Setup world
    hitable* list[2];
    list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f);
    hitable* world = new hitable_list(list, 2);
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            float u = float(i) / float(width);
            float v = float(j) / float(height);
            ray r(origin, lower_left + u * horizontal + v * vertical);
            //vec3 p = r.point_at_parameter(2.0f);
            vec3 pixel = color(r, world);
            int ir = (255.99 * pixel.r());
            int ig = (255.99 * pixel.g());
            int ib = (255.99 * pixel.b());
            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    delete world;
    delete list[0];
    delete list[1];
    file.close();
    return 0;
}