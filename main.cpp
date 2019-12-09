#include <iostream>
#include <fstream>
#include <math.h>
#include <Eigen/Dense>
#include <limits>
#include <random>
#include <thread>

#include "norm_renderer.hpp"
#include "gi_renderer.hpp"
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
#include "rectangle.hpp"
#include "diffuse_light.hpp"
#include "flip_normals.hpp"

using namespace Eigen;

#define IMG_WIDTH 200
#define IMG_HEIGHT 200
#define N_SAMPLES 50

hittable *cornell_box() {
	material *red = new lambertian(Vector3f(0.65, 0.05, 0.05));
	material *white = new lambertian(Vector3f(0.73, 0.73, 0.73));
	material *green = new lambertian(Vector3f(0.12, 0.45, 0.15));
	material *light = new diffuse_light(Vector3f(15, 15, 15));

	Vector3f offset(.28, -2.6, -2.5);

	mesh **meshes = new mesh*[8];
	int n = 0;
	// Walls
	meshes[n++] = new mesh(offset, "leftwall.obj", green);
	meshes[n++] = new mesh(offset, "rightwall.obj", red);
	meshes[n++] = new mesh(offset, "backwall.obj", white);
	meshes[n++] = new mesh(offset, "ceiling.obj", white);
	meshes[n++] = new mesh(offset, "floor.obj", white);
	// Boxes	
	meshes[n++] = new mesh(offset, "tallbox.obj", white);
	meshes[n++] = new mesh(offset, "shortbox.obj", white);
	// Light
	meshes[n++] = new mesh(offset, "light.obj", light);
	bvh_node *bvh = new bvh_node(meshes[0]->list, meshes[0]->size);
	for (int i = 1; i < n; ++i) {
		bvh_node *node = new bvh_node(meshes[i]->list, meshes[i]->size);
		bvh = bvh->merge(node);
			
	}
	return bvh;
}

void render(int tid, int nthreads, camera cam, hittable *world, Vector3f **img, renderer *rend)
{
	for (int j = IMG_HEIGHT - tid - 1; j >= 0; j -= nthreads) {
		img[j] = new Vector3f[IMG_WIDTH];
		for (int i = 0; i < IMG_WIDTH; ++i) {
			Vector3f col(0, 0, 0);
			for (int s = 0; s < rend->n_samples; s++) {
				float u = float(i + drand48()) / IMG_WIDTH;
				float v = float(j + drand48()) / IMG_HEIGHT;
				ray r = cam.get_ray(u, v);
				Vector3f throughput = Vector3f(1,1,1);
				col += rend->compute_color(r, world, 0, throughput);
			}
			col /= float(rend->n_samples);
			img[j][i] = Vector3f(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
		}
	}
}

int main()
{
	Vector3f lookfrom(0, 0, 0);
	Vector3f lookat(0,0,-1);
	Vector3f up(0,1,0);
	float vfov = 90.0;

	camera cam(lookfrom, lookat, up, vfov,
			float(IMG_WIDTH)/float(IMG_HEIGHT));

	hittable *world = cornell_box();
	Vector3f **img = new Vector3f*[IMG_HEIGHT];
	renderer *r = new norm_renderer();
	/* renderer *r = new gi_renderer(N_SAMPLES); */

	int nthreads = std::thread::hardware_concurrency();
	std::cout << "Rendering with " << nthreads << " threads" << std::endl;
	std::thread threads[nthreads];

	// Initialize threads to render image
	for (int i = 0; i < nthreads; ++i)
		threads[i] = std::thread(render, i, nthreads, cam, world, img, r);

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
