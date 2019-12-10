#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "aabb.hpp"
#include "material.hpp"
#include "sampling.hpp"

class sphere : public hittable
{
	public:
		sphere() {};
		sphere(Eigen::Vector3f c, float r, material* m) : center(c), radius(r), mat(m) {};
		virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
		Eigen::Vector3f random_sample(const Eigen::Vector3f& origin) {
			Eigen::Vector3f dir = center - origin;
			float e1 = drand48();
			float e2 = drand48();
			float z = 1 + e2*(sqrt(1-pow(radius/dir.norm(), 2)) - 1);
			float phi = 2*M_PI*e1;
			float x = cos(phi)*sqrt(1-z*z);
			float y = sin(phi)*sqrt(1-z*z);

			return change_basis(Eigen::Vector3f(x, y, z), dir);
		}

		float pdf(const Eigen::Vector3f& origin, const Eigen::Vector3f& dir) {
			hit_record rec;
			float tmin = 0.001;
			if (this->hit(ray(origin, dir), tmin, std::numeric_limits<float>::max(), rec)) {
				float cos_theta_max = sqrt(1 - pow(radius/(center-origin).norm(), 2));
				float solid_angle = 2*M_PI*(1-cos_theta_max);
				return  1 / solid_angle;
			}
			else
				return 0;
		}

		virtual bool bounding_box(aabb& box) const
		{
			box = aabb(center - Eigen::Vector3f(radius, radius, radius),
					center + Eigen::Vector3f(radius, radius, radius));
			return true;
		}
		Eigen::Vector3f center;
		float radius;
		material* mat;
};



bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const
{
	Eigen::Vector3f oc = r.origin() - center;
	Eigen::Vector3f dir = r.direction();
	float a = dir.dot(dir);
	float b = oc.dot(dir);
	float c = oc.dot(oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float t1 = (-b - sqrt(b*b-a*c)/a);
		float t2 = (-b + sqrt(b*b-a*c)/a);
		if (t1 < tmax && t1 > tmin) {
			rec.t = t1;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat = mat;
			return true;
		}
		if (t2 < tmax && t2 > tmin) {
			rec.t = t2;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat = mat;
			return true;
		}
	}
	return false;
}


#endif
