#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "ray.hpp"
#include "material.hpp"

float schlick(float cosine, float ref_idx)
{
	float r0 = (1-ref_idx) / (1+ref_idx);
	r0 = r0*r0;
	return r0 + (1-r0)*pow((1-cosine), 5);
}

bool refract(const Eigen::Vector3f& v, const Eigen::Vector3f& n, float ni_over_nt, Eigen::Vector3f& refracted)
{
	Eigen::Vector3f uv = v.normalized();
	float dt = uv.dot(n);
	float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
	if (discriminant > 0) {
		refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
		return true;
	}
	return false;
}

class dielectric : public material {
	public:
		dielectric(float ri) : ref_idx(ri) {}


		virtual bool scatter(const ray& r_in, const hit_record& rec, Eigen::Vector3f& attenuation, ray& scattered) const 
		{
			Eigen::Vector3f outward_normal;
			Eigen::Vector3f reflected = reflect(r_in.direction(), rec.normal);
			float ni_over_nt;
			attenuation = Eigen::Vector3f(1, 1, 1);
			Eigen::Vector3f refracted;
			float reflect_prob;
			float cosine;
			if (rec.normal.dot(r_in.direction()) > 0) {
				outward_normal = -rec.normal;
				ni_over_nt = ref_idx;
				cosine = ref_idx * rec.normal.dot(r_in.direction()) / r_in.direction().norm();
			}
			else {
				outward_normal = rec.normal;
				ni_over_nt = 1.0 / ref_idx;
				cosine = -rec.normal.dot(r_in.direction()) / r_in.direction().norm();
			}

			if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
				reflect_prob = schlick(cosine, ref_idx);
			else
				reflect_prob = 1.0;

			if (drand48() < reflect_prob)
				scattered = ray(rec.p, reflected);
			else
				scattered = ray(rec.p, refracted);

			return true;
		}
		float ref_idx;
};

#endif
