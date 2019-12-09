#ifndef GI_RENDERER_HPP
#define GI_RENDERER_HPP

#include "material.hpp"
#include "sampling.hpp"

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
				float material_pdf;
				// Scatter incident ray (dependent on surface material) to get exitant ray
				if (rec.mat->scatter(r, rec, attenuation, scattered, material_pdf)) {
					/* if (depth > max_depth) */
					/* 	return emitted; */

					// Generate random scatter direction wrt some probability density function
					Eigen::Vector3f random_dir = change_basis(uniform_sample_proj_solid_angle(), rec.normal);
					scattered = ray(rec.p, random_dir);
					float cos = random_dir.dot(rec.normal);
					float pdf = 0;
					if (cos > 0)
						pdf = cos / M_PI;

					// Attentuate throughput
					if (depth > 1)
						throughput = material_pdf*attenuation.cwiseProduct(throughput)/pdf;
					// Russian Roulette path termination
					float p = std::max(throughput[0], std::max(throughput[1], throughput[2]));
					if (drand48() > p)
						return (1/p)*(emitted);
					else {
						// Color = emitted + albedo * reflected * scatter_pdf / random_direction_pdf
						Eigen::Vector3f reflected = emitted + material_pdf*attenuation.cwiseProduct(compute_color(scattered, objects, depth +1, throughput))/pdf;
						return (1/p)*(reflected);
					}
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
