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
			/* Eigen::Vector3f dir = uniform_sample_proj_solid_angle(); */
			// Construct basis vectors
			/* Eigen::Vector3f temp = Eigen::Vector3f(0, 0, 1); */
			/* Eigen::Vector3f w = rec.normal; */
			/* Eigen::Vector3f u = w.cross(temp); */
			/* Eigen::Vector3f v = w.cross(u); */

			/* // Need to adjust PSA to frame coordinates */
			/* Eigen:Vector3f target; */
			/* float x = dir.x, y = dir.y, z = dir.z; */
			/* target.x = x * u.x + y * v.x + z * w.x; */
			/* target.y = x * u.y + y * v.y + z * w.y; */
			/* target.z = x * u.z + y * v.z + z * w.z; */

			/* Eigen:Vector3f target = rec.p + target; */

			Eigen::Vector3f target = rec.p + rec.normal + uniform_sample_unit_sphere();
			scattered = ray(rec.p, target - rec.p);
			attenuation = albedo;
			return true;
		}
		Eigen::Vector3f albedo;
};

#endif
