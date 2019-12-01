#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "material.hpp"

class triangle : public hittable
{
public:
	triangle() {};
	triangle(Eigen::Vector3f v1, Eigen::Vector3f v2, Eigen::Vector3f v3, material* m) : v1(v1), v2(v2), v3(v3), mat(m) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const
	{
		// Plane defined by vector in the plane and normal
		Eigen::Vector3f vec1 = v1-v2;
		Eigen::Vector3f vec2 = v3-v2;
		Eigen::Vector3f normal = vec1.cross(vec2); // Careful in how you define the normal here
		// Normalize just in case

		// Compute ray/plane intersection point
		float t = (v1-r.origin()).dot(normal) / r.direction().dot(normal)
		if (t < tmin || t > tmax)
			return false;

		Eigen::Vector3f p = r.point_at_parameter(t);

		// Inside-outside test
		Eigen::Vector3f A = ((v1-v2).cross(rec.p-v2));
		Eigen::Vector3f B = ((v3-v1).cross(rec.p-v1));
		Eigen::Vector3f C = ((v2-v3).cross(rec.p-v3));
		if (A.dot(normal) < 0 || B.dot(normal) < 0 || C.dot(normal) < 0)
			return false;

		rec.t = t;
		rec.mat = mat;
		rec.normal = normal;
		rec.p = p;
		return true;
	}

	virtual bool bounding_box(float t0, float t1, aabb& box) const
	{
		// Construct AABB from front, left, bot corner and back, right top corner
		float xmin, xmax, ymin, ymax, zmin, zmax;
		xmin = min(v1.x, v2.x, v3.x);
		xmax = max(v1.x, v2.x, v3.x);
		ymin = min(v1.y, v2.y, v3.y);
		ymax = max(v1.y, v2.y, v3.y);
		zmin = min(v1.z, v2.z, v3.z);
		zmax = max(v1.z, v2.z, v3.z);
		box = aabb(Eigen::Vector3f(xmin, ymin, zmin), Eigen::Vector3f(xmax, ymax, zmax));
		return true;
	}
	Eigen::Vector3f v1;
	Eigen::Vector3f v2;
	Eigen::Vector3f v3;
	material* mat;
};

#endif
