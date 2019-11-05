#include <iostream>
#include <fstream>
#include <math.h>
#include <Eigen/Dense>
#include <limits>
#include <random>

#include "camera.hpp"
#include "hittable_list.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "dielectric.hpp"

using namespace Eigen;

Vector3f color(const ray& r, hittable *world, int depth)
{
	hit_record rec;
	float epsilon = 0.001;
	int max_depth = 50;	
	if (world->hit(r, epsilon, std::numeric_limits<float>::max(), rec)) {
		ray scattered;
		Vector3f attenuation;
		if (depth < max_depth && rec.mat->scatter(r, rec, attenuation, scattered))
			return attenuation.cwiseProduct(color(scattered, world, depth+1));
		else
			return Vector3f(0, 0, 0);
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

	camera cam(Eigen::Vector3f(-2,2,1), Eigen::Vector3f(0,0,-1), Eigen::Vector3f(0,1,0), 90, float(nx)/ny);

	hittable *list[4];
	list[0] = new sphere(Vector3f(0, 0, -1), 0.5, new lambertian(Vector3f(0.1, 0.2, 0.5)));
	list[1] = new sphere(Vector3f(0, -100.5, -1), 100, new lambertian(Vector3f(0.8, 0.8, 0.0)));
	list[2] = new sphere(Vector3f(1, 0, -1), 0.5, new metal(Vector3f(0.8, 0.6, 0.2)));
	list[3] = new sphere(Vector3f(-1, 0, -1), 0.5, new dielectric(1.5));
	hittable *world = new hittable_list(list, 4);
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; ++i) {
			Vector3f col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + drand48()) / nx;
				float v = float(j + drand48()) / ny;
				ray r = cam.get_ray(u, v);
				Vector3f p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
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
