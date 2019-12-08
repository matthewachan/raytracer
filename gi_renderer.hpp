#ifndef GI_RENDERER_HPP
#define GI_RENDERER_HPP

#include "material.hpp"

class gi_renderer : public renderer {
	public:
		gi_renderer(int n) {
			this->n_samples = n;
		};

		virtual Eigen::Vector3f compute_color(const ray& r, hittable *objects, int depth, Eigen::Vector3f throughput) const {
			hit_record rec;
			float epsilon = 0.001;
			if (objects->hit(r, epsilon, std::numeric_limits<float>::max(), rec)) {

				// Check if we hit a light source / emitter
				Eigen::Vector3f emitted = rec.mat->emitted(r, rec);

				Eigen::Vector3f attenuation;
				ray scattered;
				float pdf;
				// Scatter incident ray (dependent on surface material) to get exitant ray
				if (rec.mat->scatter(r, rec, attenuation, scattered, pdf)) {
					
					
					if (depth > max_depth)
						return emitted;
					// Attentuate throughput
					/* throughput = attenuation.cwiseProduct(throughput); */

					// Russian Roulette path termination
					/* float p = std::max(throughput[0], std::max(throughput[1], throughput[2])); */
					/* if (drand48() > p) */
					/* 	return emitted; */
					/* throughput *= 1/p; */

					return emitted + attenuation.cwiseProduct(compute_color(scattered, objects, depth +1, throughput));
				}
				else
					return emitted;
			}

			// Return background color
			return Eigen::Vector3f(0, 0, 0);
		};

		int max_depth = 5;

};

#endif
