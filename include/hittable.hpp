#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"
#include "aabb.hpp"

class material;

struct hit_record {
	float t;
	Eigen::Vector3f p;
	Eigen::Vector3f normal;
	material *mat;
};

class hittable
{
	public:
		virtual bool hit(const ray&r, float tmin, float tmax, hit_record& rec) const = 0;
		virtual bool bounding_box(aabb& box) const = 0;
};

#endif
