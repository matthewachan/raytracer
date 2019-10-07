#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include "hittable.hpp"

class hittable_list : public hittable
{
	public:
		hittable_list() {}
		hittable_list(hittable **l, int n) {list = l; list_size = n;}
		virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
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
