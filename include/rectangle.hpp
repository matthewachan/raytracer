#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <assert.h>

class rectangle : public hittable {
	rectangle(Eigen::Vector3f c1, Eigen::Vector3f c2, material* m) : bot_corner(c1), top_corner(c2), mat(m) {

		float thresh = 0.00001;
		assert(c1[0] - c2[0] < thresh || c1[1] - c2[1] < thresh || c1[2] - c2[2] < thresh);
		if (c1[0] - c2[0] < thresh)
			i = 0;
		else if (c1[1] - c2[1] < thresh)
			i = 1;
		else
			i = 2;
	}
	virtual bool bounding_box(aabb& box) const {
		float offset = 0.00001;

		Eigen::Vector3f c1 = bot_corner;
		Eigen::Vector3f c2 = top_corner;
		c1[i] -= offset;
		c2[i] -= offset;
		box = aabb(c1, c2);
		return true;
	}
	bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
		float t = (top_corner[i]-r.origin()[i]) / r.direction()[i];
		if (t < tmin || t > tmax)
			return false;
		int a = (i + 1) % 3;
		int b = (i + 2) % 3;
		float p1 = r.origin()[a] + t*r.direction()[a];
		float p2 = r.origin()[b] + t*r.direction()[b];
		if (p1 < bot_corner[a] || p1 > top_corner[a] || p2 < bot_corner[b] || p2 > top_corner[b])
			return false;
		rec.t = t;
		rec.mat = mat;
		rec.p = r.point_at_parameter(t);
		rec.normal = Eigen::Vector3f(0, 0, 1);
		return true;
	}

	Eigen::Vector3f bot_corner, top_corner;
	int i;
	material *mat;
};

#endif
