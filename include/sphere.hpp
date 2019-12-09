#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "aabb.hpp"
#include "material.hpp"

class sphere : public hittable
{
	public:
		sphere() {};
		sphere(Eigen::Vector3f c, float r, material* m) : center(c), radius(r), mat(m) {};
		virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
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
