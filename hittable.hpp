#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"

struct hit_record {
	float t;
	Eigen::Vector3f p;
	Eigen::Vector3f normal;
};

class hittable
{
	public:
		virtual bool hit(const ray&r, float tmin, float tmax, hit_record& rec) const = 0;
};
	

#endif
