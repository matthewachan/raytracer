#ifndef XZ_PLANE_HPP
#define XZ_PLANE_HPP

class xz_plane: public hittable {
    public:
        xz_plane() {}
        xz_plane(float x0, float x1, float z0, float z1, float k, material *mat)
            : x0(x0), x1(x1), z0(z0), z1(z1), k(k), mat(mat) {};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        virtual bool bounding_box(aabb& box) const {
		float offset = 0.00001;
		Eigen::Vector3f c1 = Eigen::Vector3f(x0, k-offset, z0);
		Eigen::Vector3f c2 = Eigen::Vector3f(z1, k+offset, z1);
		box = aabb(c1, c2);
		return true;
	}
	material *mat;
	float x0, x1, z0, z1, k;
};

bool xz_plane::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    float t = (k-r.origin().y()) / r.direction().y();
    if (t < tmin || t > tmax)
        return false;
    float x = r.origin().x() + t*r.direction().x();
    float z = r.origin().z() + t*r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.t = t;
    rec.mat = mat;
    rec.p = r.point_at_parameter(t);
    rec.normal = Eigen::Vector3f(0, 1, 0);
    return true;
}

#endif
