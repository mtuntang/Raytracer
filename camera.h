#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer.h"
#include "color.h"
#include "hittable.h"

#include <iostream>

class camera {
public:
	double aspect_ratio = 1.0;  // Ratio of image width over height
	int    image_width = 100;  // Rendered image width in pixel count
	int    samples_per_pixel = 10;   // Count of random samples for each pixel
	int    max_depth = 10;   // Maximum number of ray bounces into scene

	void render(const hittable& world) {
		initialize();
		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int j = 0; j < image_height; ++j) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; ++i) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; ++sample) {
					ray r = get_random_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				write_color(std::cout, pixel_color, samples_per_pixel);
			}
		}

		std::clog << "\rDone.                 \n";
	}

private:
	int    image_height;   // Rendered image height
	point3 center;         // Camera center
	point3 pixel00_loc;    // Location of pixel 0, 0
	vec3   pixel_delta_u;  // Offset to pixel to the right
	vec3   pixel_delta_v;  // Offset to pixel below

	void initialize() {
		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		center = point3(0, 0, 0);

		// Determine viewport dimensions.
		auto focal_length = 1.0;
		auto viewport_height = 2.0;
		auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

		// Calculate the vectors across the horizontal and down the vertical viewport edges.
		auto viewport_u = vec3(viewport_width, 0, 0);
		auto viewport_v = vec3(0, -viewport_height, 0);

		// Calculate the horizontal and vertical delta vectors from pixel to pixel.
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// Calculate the location of the upper left pixel.
		auto viewport_upper_left =
			center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	/*
	* The function is recursive and simulates the behavior of light as it scatters off surfaces.
	* This means the ray might bounce off surfaces multiple times, with each bounce represented by a recursive call.
	*/
	color ray_color(const ray& r, int depth, const hittable& world) const {
		hit_record rec;

		if (depth <= 0)
			return color(0, 0, 0);

		/*
		   Diffusion is simulated by scattering the ray in a random direction based on the surface normal.
		   The recursion applies an attenuation factor [0, 1], simulating the absorption of light with each bounce.
		   Ray bounces 100% when the surface is white, 0% bounce or completely absorbed if black.
		   The interval starts from 0.001 to avoid shadow acne caused by self-intersection from floating point issues
		 */
		if (world.hit(r, interval(0.001, infinity), rec)) {
			/* Lambertian reflection is simulated by adding a random unit vector to the surface normal,
			 introducing a bias towards the normal but still providing a computationally efficient
			 and visually plausible model of diffuse scattering compared with rejection sampling that is more uniformly distributed but 
			 more expensive due to random iterative ray generation.
			 */
			vec3 direction = rec.normal + random_unit_vector();
			auto a_factor = 0.5;
			return a_factor * ray_color(ray(rec.p, direction), depth - 1, world);
		}

		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}

	// Note: a pixel in a viewport/camera can be a block of multiple colors, hence we grab the center then offset it to get surrounding color samples
	// Get a randomly sampled camera ray for the pixel at location i,j.
	ray get_random_ray(int i, int j) const {
		auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
		auto pixel_sample = pixel_center + pixel_sample_square();

		auto ray_origin = center;
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	// Returns a random point in the square surrounding a pixel at the origin.
	vec3 pixel_sample_square() const {
		auto px = -0.5 + random_double();
		auto py = -0.5 + random_double();
		return (px * pixel_delta_u) + (py * pixel_delta_v);
	}
};

#endif