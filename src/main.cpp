#include <iostream>
#include <fstream>
#include <math.h>
#include <Eigen/Dense>
#include <limits>
#include <random>
#include <thread>

#include "norm_renderer.hpp"
#include "hittable_list.hpp"
#include "gi_renderer.hpp"
#include "bvh_node.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "lambertian.hpp"
#include "diffuse_light.hpp"

using namespace Eigen;

#define IMG_WIDTH 200
#define IMG_HEIGHT 200
#define N_SAMPLES 50

/* #include <chrono>*/
/* #include <random> // requires flag --std=c++11 (or higher)*/
/* #include <mutex> // requires flag -pthread*/

/* std::mt19937_64 engine; // Defines an engine*/
/* std::uniform_real_distribution<float> rng(0.0, 1.0); // Define distribution*/
/* std::mutex mutex_lock; // a mutex to guard my_rng*/

/* // This is the function to call if you want a random number in the interval [0,1)*/

/* double RANDOM() {*/
/*     std::lock_guard<std::mutex> lock(mutex_lock);*/
/*     return rng(engine);*/
/*     // mutex is released when lock goes out of scope*/
/* }*/


/* void Seed(int seed) {*/
/*     std::lock_guard<std::mutex> lock(my_rng_mutex);*/
/*     my_rng.seed(seed);*/
/*     // mutex is released when lock goes out of scope*/
/* }*/




hittable *cornell_box(material **mats, int n_mats, mesh **meshes, int n_meshes, hittable_list **lights) {
	material *red = new lambertian(Vector3f(0.65, 0.05, 0.05));
	material *white = new lambertian(Vector3f(0.73, 0.73, 0.73));
	material *green = new lambertian(Vector3f(0.12, 0.45, 0.15));
	material *light = new diffuse_light(Vector3f(15, 15, 15));
	mats = new material*[4];
	mats[n_mats++] = red;
	mats[n_mats++] = white;
	mats[n_mats++] = green;
	mats[n_mats++] = light;

	Vector3f offset(.28, -2.6, -2.5);

	meshes = new mesh*[8];
	// Walls
	meshes[n_meshes++] = new mesh(offset, "models/leftwall.obj", green);
	meshes[n_meshes++] = new mesh(offset, "models/rightwall.obj", red);
	meshes[n_meshes++] = new mesh(offset, "models/backwall.obj", white);
	meshes[n_meshes++] = new mesh(offset, "models/ceiling.obj", white);
	meshes[n_meshes++] = new mesh(offset, "models/floor.obj", white);
	// Boxes	
	meshes[n_meshes++] = new mesh(offset, "models/tallbox.obj", white);
	meshes[n_meshes++] = new mesh(offset, "models/shortbox.obj", white);
	// Light
	meshes[n_meshes++] = new mesh(offset, "models/light.obj", light);

	/* mesh *light_mesh = new mesh(offset, "models/light.obj", light); */
	/* hittable **l = new hittable*[1]; */
	/* l[0] = new sphere(Vector3f(0,0,0),1,white); */
	/* lights[0] = new hittable_list(light_mesh->list, light_mesh->size); */
	/* lights = new hittable_list(light_mesh->list, light_mesh->size); */

	/* hittable **l = new hittable*[1]; */
	/* l[0] = new sphere(Vector3f(0,0,0),1,white); */
	/* hittable_list *hl = new hittable_list(l, 1); */

	

	bvh_node *bvh = new bvh_node(meshes[0]->list, meshes[0]->size);
	for (int i = 1; i < n_meshes; ++i) {
		bvh_node *node = new bvh_node(meshes[i]->list, meshes[i]->size);
		bvh = bvh->merge(node);
			
	}
	return bvh;
}

void render(int tid, int nthreads, camera cam, hittable *world, hittable_list *lights, Vector3f **img, renderer *rend)
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
				col += rend->compute_color(r, world, lights, 0, throughput);
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

	material **mats;
	mesh **meshes;
	hittable_list **l = new hittable_list*[0];
	int n_mats, n_meshes;
	hittable *world = cornell_box(mats, n_mats, meshes, n_meshes, l);


	Vector3f **img = new Vector3f*[IMG_HEIGHT];
	/* renderer *r = new norm_renderer(); */
	renderer *r = new gi_renderer(N_SAMPLES);

	int nthreads = std::thread::hardware_concurrency();
	std::cout << "Rendering with " << nthreads << " threads" << std::endl;
	std::thread threads[nthreads];

	// Initialize threads to render image
	for (int i = 0; i < nthreads; ++i)
		threads[i] = std::thread(render, i, nthreads, cam, world, lights, img, r);

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

	for (int i = 0; i < n_mats; ++i)
		delete mats[i];
	for (int i = 0; i < n_meshes; ++i)
		delete meshes[i];
	delete[] mats;
	delete[] meshes;
	for (int j = IMG_HEIGHT - 1; j >= 0; j--)
		delete[] img[j];
	delete[] img;
	delete r;
	delete world;
	
	return 0;
}
