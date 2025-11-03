#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

#define TMAX 10000.0f

vec3 color(const ray &r, hitable *world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001f, TMAX, rec))
    {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
            return vec3(0.0f, 0.0f, 0.0f);
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
    int width = 800, height = 400, num_samples = 100;
    std::ofstream file("images\\output.ppm");
    file << "P3\n"
         << width << " " << height << "\n255\n";
    // Setup world
    hitable *list[4];
    list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, new lambertian(vec3(0.8f, 0.3f, 0.3f)));
    list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, new lambertian(vec3(0.8f, 0.8f, 0.0f)));
    list[2] = new sphere(vec3(1.0f, 0.0f, -1.0f), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.3f));
    list[3] = new sphere(vec3(-1.0f, 0.0f, -1.0f), 0.5f, new metal(vec3(0.8f, 0.8f, 0.8f), 1.0f));
    hitable *world = new hitable_list(list, 4);
    camera cam;
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            vec3 pixel(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < num_samples; s++)
            {
                float u = float(i + randnum()) / float(width);
                float v = float(j + randnum()) / float(height);
                ray r = cam.get_ray(u, v);
                //vec3 p = r.point_at_parameter(2.0f);
                pixel += color(r, world, 0);
            }
            pixel /= float(num_samples);
            pixel = vec3(sqrt(pixel[0]), sqrt(pixel[1]), sqrt(pixel[2]));
            int ir = (255.99 * pixel[0]);
            int ig = (255.99 * pixel[1]);
            int ib = (255.99 * pixel[2]);
            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    file.close();
    return 0;
}