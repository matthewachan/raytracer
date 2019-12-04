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

Vector3f color(const ray& r, hittable *world, int depth)
{
	hit_record rec;
	float epsilon = 0.001;
	int max_depth = 50;	
	if (world->hit(r, epsilon, std::numeric_limits<float>::max(), rec)) {
		ray scattered;
		Vector3f attenuation;
		Vector3f emitted = rec.mat->emitted();
		if (depth < max_depth && rec.mat->scatter(r, rec, attenuation, scattered))
			return emitted + attenuation.cwiseProduct(color(scattered, world, depth+1));
		else
			return emitted;
	}
	else
		return Vector3f(0, 0, 0);
}

hittable *cornell_box() {
    hittable **list = new hittable*[6];
    int i = 0;
    material *red = new lambertian(Vector3f(0.65, 0.05, 0.05));
    material *white = new lambertian(Vector3f(0.73, 0.73, 0.73));
    material *green = new lambertian(Vector3f(0.12, 0.45, 0.15));
    material *light = new diffuse_light(Vector3f(15, 15, 15));

    list[i++] = new flip_normals(new yz_plane(0, 555, 0, 555, 555, green));
    list[i++] = new yz_plane(0, 555, 0, 555, 0, red);
    list[i++] = new xz_plane(213, 343, 227, 332, 554, light);
    list[i++] = new flip_normals(new xz_plane(0, 555, 0, 555, 555, white));
    list[i++] = new xz_plane(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_plane(0, 555, 0, 555, 555, white));

    return new hittable_list(list,i);
}

int main()
{
	std::ofstream output;
	output.open("output.ppm");
	int nx = 200;
	int ny = 200;
	int ns = 100;
	output << "P3\n" << nx << " " << ny << "\n255\n";

	Vector3f lookfrom(0, 0, 1);
	Vector3f lookat(0, 0, -1);

	/* Cornell box camera settings */
	/* Vector3f lookfrom(278, 278, -800); */
	/* Vector3f lookat(278,278,0); */
	/* float dist_to_focus = 10.0; */
	/* float aperture = 0.0; */
	/* float vfov = 40.0; */

	/* camera cam(lookfrom, lookat, Vector3f(0,1,0), vfov, float(nx)/float(ny)); */
	camera cam(Eigen::Vector3f(0,0,1), Eigen::Vector3f(0,0,-1), Eigen::Vector3f(0,1,0), 90, float(nx)/ny);

	hittable ** list = new hittable*[4];
	/* list[0] = new sphere(Vector3f(0,-1000, 0), 1000, new lambertian(Vector3f(0.1, 0.2, 0.5))); */
	/* list[1] = new sphere(Vector3f(0, 2, 0), 2, new lambertian(Vector3f(0.8, 0.8, 0))); */
	/* list[2] = new sphere(Vector3f(0, 7, 0), 2, */
	/* 		new diffuse_light(Vector3f(4,4,4))); */
	/* list[3] = new xyrect(3, 5, 1, 3, -2, */
	/* 		new diffuse_light(Vector3f(3, 3, 3))); */
	list[0] = new sphere(Vector3f(0, 0, -1), 0.5, new lambertian(Vector3f(0.1, 0.2, 0.5)));
	list[1] = new sphere(Vector3f(0, -100.5, -1), 100, new lambertian(Vector3f(0.8, 0.8, 0.0)));
	list[2] = new triangle(Vector3f(-1, -1, -2), Vector3f(-1, 1, -2), Vector3f(1, 1, -2), new lambertian(Vector3f(1, 0, 0)));
	/* list[2] = new sphere(Vector3f(1, 0, -1), 0.5, new metal(Vector3f(0.8, 0.6, 0.2))); */
	/* list[3] = new sphere(Vector3f(-1, 0, -1), 0.5, new dielectric(1.5)); */
	list[3] = new sphere(Vector3f(-1, 2, -1), 0.5, new diffuse_light(Vector3f(24,24,24)));
	/* hittable **list = new hittable*[2]; */
	/* list[0] = new triangle(Vector3f(0, 2, 0), Vector3f(1, 2, 0), Vector3f(4, 4, 0), new lambertian(Vector3f(0.1, 0.2, 0.5))); */
	/* list[1] = new sphere(Vector3f(0, 2, 0), 2, new lambertian(Vector3f(0.8, 0.8, 0))); */
	hittable *world = new hittable_list(list, 4);


	/* hittable *world = cornell_box(); */
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
