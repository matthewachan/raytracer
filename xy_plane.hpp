#ifndef XY_PLANE_HPP
#define XY_PLANE_HPP 

class xy_plane : public hittable {
public:
xy_plane() {}
xy_plane(float x0, float x1, float y0, float y1, float k, material *mat)
	: x0(x0), x1(x1), y0(y0), y1(y1), k(k), mat(mat) {}
virtual bool hit(const ray&r, float tmin, float tmax, hit_record& rec) const;
virtual bool bounding_box(float tmin, float tmax, aabb& box) const
{
	float offset = 0.00001;
	Eigen::Vector3f c1 = Eigen::Vector3f(x0, y0, k-offset);
	Eigen::Vector3f c2 = Eigen::Vector3f(x1, y1, k+offset);
	box = aabb(c1, c2);
	return true;
}


material *mat;
float x0, x1, y0, y1, k;
};

bool xy_plane::hit(const ray& r, float tmin, float tmax, hit_record& rec) const
{
	float t = (k-r.origin().z()) / r.direction().z();
	if (t < tmin || t > tmax)
		return false;
	float x = r.origin().x() + t*r.direction().x();
	float y = r.origin().y() + t*r.direction().y();
	if (x < x0 || x > x1 || y < y0 || y > y1)
		return false;
	rec.t = t;
	rec.mat = mat;
	rec.p = r.point_at_parameter(t);
	rec.normal = Eigen::Vector3f(0, 0, 1);
	return true;
}

#endif
