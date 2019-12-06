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

			// Construct basis vectors
			Eigen::Vector3f w = rec.normal;
			Eigen::Vector3f u = Eigen::Vector3f();
			if (fabs(w[0]) > 0.9)
				u = Eigen::Vector3f(0, 1, 0);
			else
				u = Eigen::Vector3f(1, 0, 0);

			Eigen::Vector3f v = w.cross(u).normalized();
			u = w.cross(v);

			Eigen::Vector3f target;
			float x = dir[0], y = dir[1], z = dir[2];
			target[0] = x * u[0] + y * v[0] + z * w[0];
			target[1] = x * u[1] + y * v[1] + z * w[1];
			target[2] = x * u[2] + y * v[2] + z * w[2];

			scattered = ray(rec.p, target);
			attenuation = albedo;
			pdf = rec.normal.dot(scattered.direction()) / M_PI;

			return true;
		}
		Eigen::Vector3f albedo;
};

#endif
