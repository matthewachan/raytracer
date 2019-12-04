#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "material.hpp"

class triangle : public hittable
{
public:
	triangle() {};
	triangle(Eigen::Vector3f v0, Eigen::Vector3f v1, Eigen::Vector3f v2, material* m) : v0(v0), v1(v1), v2(v2), mat(m) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const
	{
		Eigen::Vector3f v0v1 = v1 - v0; 
		Eigen::Vector3f v0v2 = v2 - v0; 
		Eigen::Vector3f pvec = r.direction().cross(v0v2); 
		float det = v0v1.dot(pvec); 
		float invDet = 1 / det;

		Eigen::Vector3f tvec = r.origin() - v0;
		float u = tvec.dot(pvec) * invDet;
		if (u < 0 || u > 1) return false;

		Eigen::Vector3f qvec = tvec.cross(v0v1);
		float v = r.direction().dot(qvec) * invDet;
		if (v < 0 || u + v > 1) return false;

		float t = v0v2.dot(qvec) * invDet;
		Eigen::Vector3f p = r.point_at_parameter(t);
		rec.t = t;
		rec.mat = mat;
		rec.normal = v0v1.cross(v0v2);
		rec.p = p;

		return true;




		/* // Plane defined by vector in the plane and normal */
		/* Eigen::Vector3f vec1 = v1-v2; */
		/* Eigen::Vector3f vec2 = v3-v2; */
		/* Eigen::Vector3f normal = vec1.cross(vec2); // Careful in how you define the normal here */
		/* // Normalize just in case */

		/* // Compute ray/plane intersection point */
		/* float t = (v1-r.origin()).dot(normal) / r.direction().dot(normal); */
		/* if (t < tmin || t > tmax) */
		/* 	return false; */

		/* Eigen::Vector3f p = r.point_at_parameter(t); */

		/* // Inside-outside test */
		/* Eigen::Vector3f A = ((v1-v2).cross(rec.p-v2)); */
		/* Eigen::Vector3f B = ((v3-v1).cross(rec.p-v1)); */
		/* Eigen::Vector3f C = ((v2-v3).cross(rec.p-v3)); */
		/* if (A.dot(normal) < 0 || B.dot(normal) < 0 || C.dot(normal) < 0) */
		/* 	return false; */

		/* rec.t = t; */
		/* rec.mat = mat; */
		/* rec.normal = normal; */
		/* rec.p = p; */
		/* return true; */
	}

	

	virtual bool bounding_box(float t0, float t1, aabb& box) const
	{
		// Construct AABB from front, left, bot corner and back, right top corner
		float xmin, xmax, ymin, ymax, zmin, zmax;
		xmin = std::min(std::min(v1[0], v2[0]), v0[0]);
		xmax = std::max(std::max(v1[0], v2[0]), v0[0]);
		ymin = std::min(std::min(v1[1], v2[1]), v0[1]);
		ymax = std::max(std::max(v1[1], v2[1]), v0[1]);
		zmin = std::min(std::min(v1[2], v2[2]), v0[2]);
		zmax = std::max(std::max(v1[2], v2[2]), v0[2]);
		box = aabb(Eigen::Vector3f(xmin, ymin, zmin), Eigen::Vector3f(xmax, ymax, zmax));
		return true;
	}
	Eigen::Vector3f v0;
	Eigen::Vector3f v1;
	Eigen::Vector3f v2;
	material* mat;
};

#endif
