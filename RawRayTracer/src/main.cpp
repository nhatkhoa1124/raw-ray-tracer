#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../external/stb/stb_image_write.h"
#include <vector>
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

hitable *random_scene()
{
    int n = 100; // Reduced from 500 to 100
    hitable **list = new hitable *[n + 1];
    list[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
    int i = 1;

    // Reduced grid size: from -6 to 6 instead of -11 to 11
    for (int a = -6; a < 6; a++)
    {
        for (int b = -6; b < 6; b++)
        {
            // Stop if we've reached our limit (including the 3 large spheres we'll add later)
            if (i >= n - 3)
                break;

            float choose_mat = randnum();
            vec3 center(a + 0.9f * randnum(), 0.2f, b + 0.9f * randnum());

            // Only create sphere if it doesn't overlap with the center area
            if ((center - vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
            {
                if (choose_mat < 0.8f)
                { // diffuse
                    list[i++] = new sphere(center, 0.2f, new lambertian(vec3(randnum() * randnum(), randnum() * randnum(), randnum() * randnum())));
                }
                else if (choose_mat < 0.95f)
                { // metal
                    list[i++] = new sphere(center, 0.2f, new metal(vec3(0.5f * (1 + randnum()), 0.5f * (1 + randnum()), 0.5f * (1 + randnum())), 0.5 * randnum()));
                }
                else
                { // glass
                    list[i++] = new sphere(center, 0.2f, new dielectric(1.5f));
                }
            }

            // Extra safety check
            if (i >= n - 3)
                break;
        }
        // Extra safety check
        if (i >= n - 3)
            break;
    }

    // Add the 3 large spheres (total becomes exactly 100)
    list[i++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.5f));
    list[i++] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(vec3(0.4f, 0.2f, 0.1f)));
    list[i++] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));

    return new hitable_list(list, i); // i should be exactly 100 now
}

int main()
{
    int width = 800, height = 400, channels = 3, num_samples = 100;
    std::vector<unsigned char> image_data(width * height * channels);
    // Setup world
    hitable *world = random_scene();

    // Setup camera
    vec3 lookfrom(0.0f, 1.5f, 6.3f);
    vec3 lookat(0.0f, 0.9f, -1.0f);
    float dist_to_focus = (lookfrom - lookat).length();
    float aperture = 0.1f;
    camera cam(lookfrom, lookat, vec3(0.0f, 1.0f, 0.0f), 25.0f, float(width) / float(height), aperture, dist_to_focus);
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
                pixel += color(r, world, 0);
            }
            pixel /= float(num_samples);
            pixel = vec3(sqrt(pixel[0]), sqrt(pixel[1]), sqrt(pixel[2]));
            int idx = ((height - 1 - j) * width + i) * channels;
            image_data[idx + 0] = static_cast<unsigned char>(255.999 * pixel.x());
            image_data[idx + 1] = static_cast<unsigned char>(255.999 * pixel.y());
            image_data[idx + 2] = static_cast<unsigned char>(255.999 * pixel.z());
        }
    }
    stbi_write_png("images//output.png", width, height, channels, image_data.data(), width * channels);

    return 0;
}