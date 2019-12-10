#ifndef GI_RENDERER_HPP
#define GI_RENDERER_HPP

#include "material.hpp"
#include "sampling.hpp"

class gi_renderer : public renderer {
	public:
		gi_renderer(int n) {
			this->n_samples = n;
		};

		virtual Eigen::Vector3f compute_color(const ray& r, hittable *objects, hittable_list *lights, int depth, Eigen::Vector3f throughput) const {
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
					int n_lights = lights->list_size;
					int random_pick = (int) (drand48() * (n_lights+1));
					if (random_pick == n_lights) {
						Eigen::Vector3f random_dir = change_basis(uniform_sample_proj_solid_angle(), rec.normal);
						scattered = ray(rec.p, random_dir);
					} else
						scattered = ray(rec.p, lights->list[random_pick]->random_sample(rec.p));

					float weight = 1 / (n_lights + 1);
					float pdf = 0;
					for (int i = 0; i < n_lights; ++i)
						pdf += weight * lights->list[i]->pdf(rec.p, scattered.direction());
					float cos = scattered.direction().dot(rec.normal);
					if (cos > 0)
						pdf += weight * (cos / M_PI);


					// Attentuate throughput
					if (depth > 1)
						throughput = material_pdf*attenuation.cwiseProduct(throughput)/pdf;
					// Russian Roulette path termination
					float p = std::max(throughput[0], std::max(throughput[1], throughput[2]));
					if (drand48() > p)
						return (1/p)*(emitted);
					else {
						// Color = emitted + albedo * reflected * scatter_pdf / random_direction_pdf
						Eigen::Vector3f reflected = emitted + material_pdf*attenuation.cwiseProduct(compute_color(scattered, objects, lights, depth +1, throughput))/pdf;
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
