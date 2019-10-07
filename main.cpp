#include <iostream>
#include <fstream>
#include <math.h>
#include <Eigen/Dense>
#include <limits>

#include "ray.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"

using namespace Eigen;

Vector3f color(const ray& r, hittable *world)
{
	hit_record rec;
	if (world->hit(r, 0.0, std::numeric_limits<float>::max(), rec))
		return 0.5 * Vector3f(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
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
			float u = float(i) / nx;
			float v = float(j) / ny;
			ray r = cam.get_ray(u, v);
			Vector3f p = r.point_at_parameter(2.0);
			Vector3f col = color(r, world);
			int ir = int (255.99 * col[0]);
			int ig = int (255.99 * col[1]);
			int ib = int (255.99 * col[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	return 0;
}
