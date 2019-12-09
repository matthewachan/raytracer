#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "ray.hpp"
#include "sampling.hpp"
#include "material.hpp"

class lambertian : public material
{
	public:
		lambertian(const Eigen::Vector3f& a) : albedo(a) {}
		virtual bool scatter(const ray& r_in, const hit_record& rec, Eigen::Vector3f& attenuation, ray& scattered, float& pdf) const
		{
			Eigen::Vector3f dir = uniform_sample_proj_solid_angle();
			scattered = ray(rec.p, change_basis(dir, rec.normal));
			attenuation = albedo;
			// brdf = attenuation * pdf / cos(theta)
			// pdf = brdf * cos(theta) / attentuation
			pdf = rec.normal.dot(scattered.direction()) / M_PI;

			return true;
		}
		Eigen::Vector3f albedo;
};

#endif
