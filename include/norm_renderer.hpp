#ifndef NORM_RENDERER_HPP
#define NORM_RENDERER_HPP

#include "renderer.hpp"

class norm_renderer : public renderer {
public:
	norm_renderer() {
		n_samples = 1;
	}

	virtual Eigen::Vector3f compute_color(const ray& r, hittable *objects, hittable_list *lights, int depth, Eigen::Vector3f throughput) const {
		hit_record rec;
		float epsilon = 0.001;
		if (objects->hit(r, epsilon, std::numeric_limits<float>::max(), rec))
			return 0.5 * (rec.normal + Eigen::Vector3f(1,1,1));
		return Eigen::Vector3f(0, 0, 0);
	};

};

#endif
