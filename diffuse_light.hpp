#ifndef DIFFUSE_LIGHT_HPP
#define DIFFUSE_LIGHT_HPP

class diffuse_light : public material {
	public:
		diffuse_light(Eigen::Vector3f color) : color(color) {}
		virtual bool scatter(const ray& r_in, const hit_record& rec,
			Eigen::Vector3f& attenuation, ray& scattered) const
		{
			// Light sources are set to not scatter light
			return false;
		}
		virtual Eigen::Vector3f emitted() const
		{
			return color;
		}

		Eigen::Vector3f color;
};

#endif
