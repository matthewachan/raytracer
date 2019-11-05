#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include "hittable.hpp"

class hittable_list : public hittable
{
	public:
		hittable_list() {}
		hittable_list(hittable **l, int n) {list = l; list_size = n;}
		virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
		virtual bool bounding_box(float t0, float t1, aabb& box) const {
			if (list_size < 1) return false;
			aabb temp_box;
			bool first_true = list[0]->bounding_box(t0, t1, temp_box);
			if (!first_true)
				return false;
			else
				box = temp_box;
			for (int i = 1; i < list_size; i++) {
				if(list[i]->bounding_box(t0, t1, temp_box)) {
					box = surrounding_box(box, temp_box);
				}
				else
					return false;
			}
			return true;
		}
		hittable **list;
		int list_size;
};

bool hittable_list::hit(const ray& r, float tmin, float tmax, hit_record& rec) const
{
	hit_record temp;	
	bool hit_anything = false;
	double closest_so_far = tmax;
	for (int i = 0; i < list_size; ++i) {
		if (list[i]->hit(r, tmin, closest_so_far, temp)) {
			hit_anything = true;
			closest_so_far = temp.t;
			rec = temp;
		}
	}
	return hit_anything;
}

#endif
