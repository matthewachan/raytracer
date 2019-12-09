#ifndef RENDERER_HPP
#define RENDERER_HPP


#include "ray.hpp"
#include "hittable.hpp"

class renderer {
public:
	virtual Eigen::Vector3f compute_color(const ray& r, hittable *objects, int depth, Eigen::Vector3f throughput) const = 0;
	virtual ~renderer() = 0;
	int n_samples;
};
renderer::~renderer() {};

#endif