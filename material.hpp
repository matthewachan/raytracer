#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"

class material
{
	public:
		virtual bool scatter(const ray& r_in, const hit_record& rec, Eigen::Vector3f& attenuation, ray& scattered, float& pdf) const = 0;
		virtual Eigen::Vector3f emitted(const ray& r_in, const hit_record& rec) const
		{
			return Eigen::Vector3f(0, 0, 0);
		}
};

#endif
