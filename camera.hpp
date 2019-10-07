#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"

class camera
{
	public:
		camera();
		ray get_ray(float u, float v);
		Eigen::Vector3f origin;
		Eigen::Vector3f lower_left_corner;
		Eigen::Vector3f horizontal;
		Eigen::Vector3f vertical;
};

camera::camera()
{
	lower_left_corner = Eigen::Vector3f(-2.0, -1.0, -1.0);
	horizontal = Eigen::Vector3f(4.0, 0.0, 0.0);
	vertical = Eigen::Vector3f(0.0, 2.0, 0.0);
	origin = Eigen::Vector3f(0.0, 0.0, 0.0);
}

ray camera::get_ray(float u, float v)
{
	return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
}
#endif
