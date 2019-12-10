#ifndef DIFFUSE_LIGHT_HPP
#define DIFFUSE_LIGHT_HPP

class diffuse_light : public material {
	public:
		diffuse_light(Eigen::Vector3f color) : color(color) {}
		virtual bool scatter(const ray& r_in, const hit_record& rec,
				Eigen::Vector3f& attenuation, ray& scattered, float& pdf) const
		{
			// Light sources are set to not scatter light
			return false;
		}
		virtual Eigen::Vector3f emitted(const ray& r_in, const hit_record& rec) const
		{
			return color;
			/* if (rec.normal.dot(r_in.direction()) > 0.0) */
			/* 	return color; */
			/* else */
			/* 	return Eigen::Vector3f(0,0,0); */
		}

		Eigen::Vector3f color;
};

#endif
