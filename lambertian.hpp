#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "ray.hpp"
#include "sampling.hpp"
#include "material.hpp"

class lambertian : public material
{
	public:
		lambertian(const Eigen::Vector3f& a) : albedo(a) {}
		virtual bool scatter(const ray& r_in, const hit_record& rec, Eigen::Vector3f& attenuation, ray& scattered) const
		{
			Eigen::Vector3f target = rec.p + rec.normal + uniform_sample_unit_sphere();
			scattered = ray(rec.p, target - rec.p);
			attenuation = albedo;
			return true;
		}
		Eigen::Vector3f albedo;
};

#endif
