#ifndef RENDERER_HPP
#define RENDERER_HPP


#include "ray.hpp"
#include "hittable.hpp"

class renderer {
public:
	virtual Eigen::Vector3f compute_color(const ray& r, hittable *objects, Eigen::Vector3f throughput) const = 0;
	int n_samples;
};

#endif
