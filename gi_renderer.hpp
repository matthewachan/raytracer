#ifndef GI_RENDERER_HPP
#define GI_RENDERER_HPP

class gi_renderer : public renderer {
	public:
		gi_renderer(int n_samples) : n_samples(n_samples) {};

		virtual Eigen::Vector3f compute_color(const ray& r, hittable *objects, Eigen::Vector3f throughput) const {
			hit_record rec;
			float epsilon = 0.001;
			if (world->hit(r, epsilon, std::numeric_limits<float>::max(), rec)) {

				Vector3f emitted = rec.mat->emitted(r, rec);

				Vector3f attenuation;
				ray scattered;
				float pdf;
				if (rec.mat->scatter(r, rec, attenuation, scattered, pdf)) {

					throughput = attenuation.cwiseProduct(throughput);
					float p = std::max(throughput[0], std::max(throughput[1], throughput[2]));

					// Russian Roulette path termination
					if (drand48() > p)
						return emitted;

					throughput *= 1/p;

					return emitted + attenuation.cwiseProduct(color(scattered, world, throughput));
				}
				else
					return emitted;
			}

			// Return background color
			return Vector3f(0, 0, 0);
		};

};

#endif
