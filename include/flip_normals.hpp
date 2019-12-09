#ifndef FLIP_NORMALS_HPP
#define FLIP_NORMALS_HPP

class flip_normals : public hittable {
    public:
        flip_normals(hittable *p) : ptr(p) {}

        virtual bool hit(const ray& r, float t_min,
		float t_max, hit_record& rec) const
	{
            if (ptr->hit(r, t_min, t_max, rec)) {
                rec.normal = -rec.normal;
                return true;
            }
            else
                return false;
        }

        virtual bool bounding_box(aabb& box) const {
            return ptr->bounding_box(box);
        }

        hittable *ptr;
};

#endif
