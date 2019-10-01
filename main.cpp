#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include "ray.hpp"
using namespace Eigen;

Vector3f color(const ray& r)
{
	Vector3f unit_direction = r.direction().normalized();
	float t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * Vector3f(1.0f, 1.0f, 1.0f) + t * Vector3f(0.5f, 0.7f, 1.0f);
}

int main()
{
	std::ofstream output;
	output.open("output.ppm");
	int nx = 200;
	int ny = 100;
	output << "P3\n" << nx << " " << ny << "\n255\n";
	Vector3f lower_left_corner(-2.0f, -1.0f, -1.0f);
	Vector3f horizontal(4.0f, 0.0f, 0.0f);
	Vector3f vertical(0.0f, 2.0f, 0.0f);
	Vector3f origin(0.0f, 0.0f, 0.0f);
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; ++i) {
			float u = float(i) / nx;
			float v = float(j) / ny;
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			Vector3f col = color(r);
			int ir = int (255.99 * col[0]);
			int ig = int (255.99 * col[1]);
			int ib = int (255.99 * col[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	return 0;
}
