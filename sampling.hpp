#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#include <math.h>
#include <Eigen/Dense>

Eigen::Vector3f uniform_sample_unit_sphere()
{
	Eigen::Vector3f p;
	do {
		p = 2.0 * Eigen::Vector3f(drand48(), drand48(), drand48()) - Eigen::Vector3f(1, 1, 1);
	} while (pow(p.norm(), 1) >= 1.0);
	return p;
}

Eigen::Vector3f uniform_sample_proj_solid_angle()
{
	Eigen::Vector3f p;
	// Sample between 0 and 1 for z direction
	float eps1 = drand48();
	float eps2 = drand48();

	// Angle phi can lie anywhere on a circle
	float r = drand48();
	float phi = 2 * M_PI * drand48();

	// Compute radius = sin(theta)
	double xy_rad = sqrt(1 - r * r);

	p[0] = r * cos(phi);
	p[1] = r * sin(phi);
	p[2] = xy_rad;
	return p;
}

#endif
