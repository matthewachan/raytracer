#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"

class material
{
	public:
		virtual bool scatter(const ray& r_in, const hit_record& rec, Eigen::Vector3f& attenuation, ray& scattered) const = 0;
};

#endif
