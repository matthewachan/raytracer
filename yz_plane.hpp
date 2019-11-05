#ifndef YZ_PLANE_HPP
#define YZ_PLANE_HPP

class yz_plane: public hittable {
    public:
        yz_plane() {}
        yz_plane(float y0, float y1, float z0, float z1, float k, material *mat)
            : y0(y0), y1(y1), z0(z0), z1(z1), k(k), mat(mat) {};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        virtual bool bounding_box(float tmin, float tmax, aabb& box) const {
		float offset = 0.00001;
		Eigen::Vector3f c1 = Eigen::Vector3f(k-offset, y0, z0);
		Eigen::Vector3f c2 = Eigen::Vector3f(k+offset, y1, z1);
		box = aabb(c1, c2);
		return true;
	}
	material *mat;
	float y0, y1, z0, z1, k;
};

bool yz_plane::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    float t = (k-r.origin().x()) / r.direction().x();
    if (t < tmin || t > tmax)
        return false;
    float y = r.origin().y() + t*r.direction().y();
    float z = r.origin().z() + t*r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.t = t;
    rec.mat = mat;
    rec.p = r.point_at_parameter(t);
    rec.normal = Eigen::Vector3f(1, 0, 0);
    return true;
}

#endif
