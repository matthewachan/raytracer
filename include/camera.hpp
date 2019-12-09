#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <math.h>
#include "ray.hpp"

class camera
{
	public:
		camera(Eigen::Vector3f lookfrom, Eigen::Vector3f lookat, Eigen::Vector3f up, float vfov, float aspect)
		{
			Eigen::Vector3f u, v, w;
			float theta = vfov * M_PI/180;
			float half_height = tan(theta/2);
			float half_width = aspect * half_height;
			origin = lookfrom;
			w = (lookfrom-lookat).normalized();
			u = (up.cross(w)).normalized();
			v = w.cross(u);
			lower_left_corner = origin - half_width*u - half_height*v - w;
			horizontal = 2*half_width*u;
			vertical = 2*half_height*v;
		}
		ray get_ray(float u, float v);
		Eigen::Vector3f origin;
		Eigen::Vector3f lower_left_corner;
		Eigen::Vector3f horizontal;
		Eigen::Vector3f vertical;
};

/* camera::camera() */
/* { */
/* 	lower_left_corner = Eigen::Vector3f(-2.0, -1.0, -1.0); */
/* 	horizontal = Eigen::Vector3f(4.0, 0.0, 0.0); */
/* 	vertical = Eigen::Vector3f(0.0, 2.0, 0.0); */
/* 	origin = Eigen::Vector3f(0.0, 0.0, 0.0); */
/* } */

ray camera::get_ray(float u, float v)
{
	return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
}
#endif
