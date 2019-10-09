#ifndef METAL_HPP
#define METAL_HPP

#include "ray.hpp"
#include "sampling.hpp"
#include "material.hpp"

Eigen::Vector3f reflect(const Eigen::Vector3f& v, const Eigen::Vector3f& n)
{
	return v - 2*v.dot(n)*n;
}

class metal : public material
{
	public:
		metal(const Eigen::Vector3f& a) : albedo(a) {}
		virtual bool scatter(const ray& r_in, const hit_record& rec, Eigen::Vector3f& attenuation, ray& scattered) const
		{
			Eigen::Vector3f reflected = reflect(r_in.direction(), rec.normal);
			scattered = ray(rec.p, reflected);
			attenuation = albedo;
			return (rec.normal.dot(scattered.direction()) > 0);
		}
		Eigen::Vector3f albedo;
};

#endif
