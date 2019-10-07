#include <iostream>
#include <fstream>
#include <math.h>
#include <Eigen/Dense>
#include <limits>
#include <random>

#include "ray.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"

using namespace Eigen;

Vector3f uniform_sample_unit_sphere()
{
	Vector3f p;
	do {
		p = 2.0 * Vector3f(drand48(), drand48(), drand48()) - Vector3f(1, 1, 1);
	} while (pow(p.norm(), 1) >= 1.0);
	return p;
}

Vector3f color(const ray& r, hittable *world)
{
	hit_record rec;
	float epsilon = 0.001;
	if (world->hit(r, epsilon, std::numeric_limits<float>::max(), rec)) {
		Vector3f target = rec.p + rec.normal + uniform_sample_unit_sphere();
		return 0.5 * color(ray(rec.p, target - rec.p), world);
	}
	else {
		Vector3f unit_dir = r.direction();
		float t = 0.5 * (unit_dir.y() + 1);
		return (1.0 - t) * Vector3f(1, 1, 1) + t * Vector3f(0.5, 0.7, 1.0);
	}
}

int main()
{
	std::ofstream output;
	output.open("output.ppm");
	int nx = 200;
	int ny = 100;
	int ns = 100;
	output << "P3\n" << nx << " " << ny << "\n255\n";
	camera cam;
	Vector3f lower_left_corner(-2.0f, -1.0f, -1.0f);
	Vector3f horizontal(4.0f, 0.0f, 0.0f);
	Vector3f vertical(0.0f, 2.0f, 0.0f);
	Vector3f origin(0.0f, 0.0f, 0.0f);

	hittable *list[2];
	list[0] = new sphere(Vector3f(0, 0, -1), 0.5);
	list[1] = new sphere(Vector3f(0, -100.5, -1), 100);
	hittable *world = new hittable_list(list, 2);
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; ++i) {
			Vector3f col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + drand48()) / nx;
				float v = float(j + drand48()) / ny;
				ray r = cam.get_ray(u, v);
				Vector3f p = r.point_at_parameter(2.0);
				col += color(r, world);
			}
			col /= float(ns);
			col = Vector3f(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int (255.99 * col[0]);
			int ig = int (255.99 * col[1]);
			int ib = int (255.99 * col[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	return 0;
}
