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
		virtual float pdf(const Eigen::Vector3f& origin, const Eigen::Vector3f& dir) const  {return 0.0;}
        virtual Eigen::Vector3f random_sample(const Eigen::Vector3f& origin) const {return Eigen::Vector3f(1, 0, 0);}
		virtual ~hittable() = 0;
};
hittable::~hittable() {};

#endif
