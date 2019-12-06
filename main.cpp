#include <iostream>
#include <fstream>
#include <math.h>
#include <Eigen/Dense>
#include <limits>
#include <random>
#include <thread>

#include "bvh_node.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "hittable_list.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "dielectric.hpp"
#include "xy_plane.hpp"
#include "yz_plane.hpp"
#include "xz_plane.hpp"
#include "diffuse_light.hpp"
#include "flip_normals.hpp"

using namespace Eigen;

#define IMG_WIDTH 100
#define IMG_HEIGHT 100
#define N_SAMPLES 50


Vector3f color(const ray& r, hittable *world, Vector3f throughput)
{
	hit_record rec;
	float epsilon = 0.001;
	if (world->hit(r, epsilon, std::numeric_limits<float>::max(), rec)) {
		Vector3f emitted = rec.mat->emitted(r, rec);

		Vector3f attenuation;
		ray scattered;
		float pdf;
		if (rec.mat->scatter(r, rec, attenuation, scattered, pdf)) {

			throughput = attenuation.cwiseProduct(throughput);
			float p = std::max(throughput[0], std::max(throughput[1], throughput[2]));

			// Russian Roulette path termination
			if (drand48() > p)
				return emitted;

			throughput *= 1/p;

			return emitted + attenuation.cwiseProduct(color(scattered, world, throughput));
		}
		else
			return emitted;
	}

	// Return background color
	return Vector3f(0, 0, 0);
}

hittable *cornell_box() {
	hittable **list = new hittable*[8];
	int i = 0;
	material *red = new lambertian(Vector3f(0.65, 0.05, 0.05));
	material *white = new lambertian(Vector3f(0.73, 0.73, 0.73));
	material *green = new lambertian(Vector3f(0.12, 0.45, 0.15));
	material *light = new diffuse_light(Vector3f(15, 15, 15));
	list[i++] = new flip_normals(new yz_plane(0, 555, 0, 555, 555, green));
	list[i++] = new yz_plane(0, 555, 0, 555, 0, red);
	list[i++] = new xz_plane(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_plane(0, 555, 0, 555, 555, green));
	list[i++] = new xz_plane(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_plane(0, 555, 0, 555, 555, white));
	/* list[i++] = new sphere(Vector3f(278, 278, 278), 50, green); */
	list[i++] = new mesh(Vector3f(150, 150, 150), "triangle.obj", red);

	/* list[i++] = new triangle(Vector3f(150, 150, 150), Vector3f(500,500,150), Vector3f(500,150,150), red); */
	return new bvh_node(list, i);
}

void render(int tid, int nthreads, camera cam, hittable *world, Vector3f **img)
{
	for (int j = IMG_HEIGHT - tid - 1; j >= 0; j -= nthreads) {
		img[j] = new Vector3f[IMG_WIDTH];
		for (int i = 0; i < IMG_WIDTH; ++i) {
			Vector3f col(0, 0, 0);
			for (int s = 0; s < N_SAMPLES; s++) {
				float u = float(i + drand48()) / IMG_WIDTH;
				float v = float(j + drand48()) / IMG_HEIGHT;
				ray r = cam.get_ray(u, v);
				col += color(r, world, Vector3f(1, 1, 1));
			}
			col /= float(N_SAMPLES);
			img[j][i] = Vector3f(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
		}
	}
}

int main()
{
	/* Cornell box camera settings */
	Vector3f lookfrom(278, 278, -800);
	Vector3f lookat(278,278,0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;

	camera cam(lookfrom, lookat, Vector3f(0,1,0), vfov,
			float(IMG_WIDTH)/float(IMG_HEIGHT));

	hittable *world = cornell_box();
	Vector3f **img = new Vector3f*[IMG_HEIGHT];

	int nthreads = std::thread::hardware_concurrency();
	std::cout << "Rendering with " << nthreads << " threads" << std::endl;
	std::thread threads[nthreads];

	// Initialize threads to render image
	for (int i = 0; i < nthreads; ++i)
		threads[i] = std::thread(render, i, nthreads, cam, world, img);

	// Wait for threads to finish
	for (int i = 0; i < nthreads; ++i)
		threads[i].join();

	// Write image to output file
	std::ofstream output;
	output.open("output.ppm");
	output << "P3\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n255\n";
	for (int j = IMG_HEIGHT - 1; j >= 0; j--) {
		for (int i = 0; i < IMG_WIDTH; ++i) {
			Vector3f col = img[j][i];
			int ir = int (255.99 * col[0]);
			int ig = int (255.99 * col[1]);
			int ib = int (255.99 * col[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}

	return 0;
}
