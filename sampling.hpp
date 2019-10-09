#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#include <Eigen/Dense>

Eigen::Vector3f uniform_sample_unit_sphere()
{
	Eigen::Vector3f p;
	do {
		p = 2.0 * Eigen::Vector3f(drand48(), drand48(), drand48()) - Eigen::Vector3f(1, 1, 1);
	} while (pow(p.norm(), 1) >= 1.0);
	return p;
}

#endif
