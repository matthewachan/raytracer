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
		if (t > tmax || t < tmin) return false;
		Eigen::Vector3f p = r.point_at_parameter(t);
		rec.t = t;
		rec.mat = mat;
		rec.normal = (v0v1.cross(v0v2)).normalized();
		if (rec.normal.dot(r.direction()) > 0)
			rec.normal = -rec.normal;
		rec.p = p;

		return true;
	}

	Eigen::Vector3f random_sample(const Eigen::Vector3f& origin)
	{
		float e1 = drand48();
		float e2 = drand48();
		Eigen::Vector3f pt = v0 + e2 * sqrt(1 - e1) * (v1 - v0) + (1 - sqrt(1 - e1)) * (v2 - v0);
		return pt - origin;
	}

	float pdf(const Eigen::Vector3f& origin, const Eigen::Vector3f& dir) {
		hit_record rec;
		float tmin = 0.001;
		if (this->hit(ray(origin, dir), tmin, std::numeric_limits<float>::max(), rec)) {
			float area = 0.5*((v1-v0).cross(v2-v0)).norm();
			float distance_squared = pow(rec.t * dir.norm(), 2);
			float cosine = fabs(dir.dot(rec.normal) / dir.norm());
			return  distance_squared / (cosine * area);
		}
		else
			return 0;
	}


	virtual bool bounding_box(aabb& box) const
	{
		// Construct AABB from front, left, bot corner and back, right top corner
		float xmin, xmax, ymin, ymax, zmin, zmax;
		float offset = 0.00001;
		xmin = std::min(std::min(v1[0], v2[0]), v0[0]);
		xmax = std::max(std::max(v1[0], v2[0]), v0[0]);
		ymin = std::min(std::min(v1[1], v2[1]), v0[1]);
		ymax = std::max(std::max(v1[1], v2[1]), v0[1]);
		zmin = std::min(std::min(v1[2], v2[2]), v0[2]);
		zmax = std::max(std::max(v1[2], v2[2]), v0[2]);
		box = aabb(Eigen::Vector3f(xmin-offset, ymin-offset, zmin-offset), Eigen::Vector3f(xmax+offset, ymax+offset, zmax+offset));
		return true;
	}
	Eigen::Vector3f v0;
	Eigen::Vector3f v1;
	Eigen::Vector3f v2;
	material* mat;
};

#endif
