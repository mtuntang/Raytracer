#include "raytracer.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include <thread>

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}



void draw(camera cam, hittable_list world, int start, int end, std::vector<color>* out) {
    int i = (cam.image_height - start) * cam.image_width;

    for (int y = start - 1; y >= end; y--)
    {

        for (int x = 0; x < cam.image_width; x++)
        {
            color pixel_color;
            for (int s = 0; s < cam.samples_per_pixel; s++) {
                double u = (x + random_double()) / (cam.image_width - 1);
                double v = (y + random_double()) / (cam.image_height - 1);
                ray r = cam.get_private_ray_defocus((int)u, (int)v);
                pixel_color += cam.get_ray_color(r, cam.max_depth, world);
            }
            out->at(i) = pixel_color;
            i++;
        }
    }
}

int main() {
    // Old render method start
    //hittable_list world;

    //auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    //auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    //auto material_left = make_shared<dielectric>(1.50);
    //auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    //auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    //world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    //world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    //world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    //world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    //world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    //camera cam;

    //cam.aspect_ratio = 16.0 / 9.0;
    //cam.image_width = 400;
    //cam.samples_per_pixel = 100;
    //cam.max_depth = 50;

    //cam.vfov = 25;
    //// X horizontal, Y vertical, Z (+) from scrren, (-) behind of screen to viewer 
    //cam.lookfrom = point3(-2, 2, 1);
    //cam.lookat = point3(0, 0, -1);
    //cam.vup = vec3(0, 1, 0);

    //cam.defocus_angle = 10.0;
    //cam.focus_dist = 3.4;

    //cam.render(world);
    // Old render method end

    const auto aspect_ratio = 3.0 / 2.0;

    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    //camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    camera cam;

    cam.aspect_ratio = aspect_ratio;
    cam.image_width = 1200;
    cam.samples_per_pixel = 100;
    cam.max_depth = 10;

    cam.vfov = 25;
    // X horizontal, Y vertical, Z (+) from scrren, (-) behind of screen to viewer 
    cam.lookfrom = lookfrom;
    cam.lookat = vup;
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 10.0;
    cam.focus_dist = 10.0;

    std::cout << "P3\n" << cam.image_width << ' ' << cam.image_height << "\n255\n";

    hittable_list world = random_scene();
    std::vector<color> image(cam.image_height * cam.image_width);

    // Init threads

    int num_threads = 16;
    std::vector<std::thread> threads(num_threads);

    int segment = cam.image_height / num_threads;
    int segment_start = cam.image_height;
    int segment_end = segment_start - segment;

    for (int i = 0; i < num_threads; i++)
    {
        threads[i] = std::thread(draw, cam, world, segment_start, segment_end, &image);
        segment_start = segment_end;
        segment_end -= segment;
    }

    for (int i = 0; i < num_threads; i++)
    {
        threads[i].join();
    }

    std::cerr << "\nWriting...\n";

    for (color c : image)
    {
        write_color(std::cout, c, cam.samples_per_pixel);
    }

    std::cerr << "\nDone.\n";
}