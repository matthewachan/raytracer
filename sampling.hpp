#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#include <math.h>
#include <Eigen/Dense>

// Modify to take hit record and output scattered ray

Eigen::Vector3f uniform_sample_unit_sphere()
{
	Eigen::Vector3f p;
	do {
		p = 2.0 * Eigen::Vector3f(drand48(), drand48(), drand48()) - Eigen::Vector3f(1, 1, 1);
	} while (pow(p.norm(), 2) >= 1.0);
	return p;
}

Eigen::Vector3f uniform_sample_proj_solid_angle()
{

	/* float cosPhi = 2.0f * randf() - 1.0f; */
	/* float sinPhi = std::sqrt(1.0f - cosPhi * cosPhi); */
	/* float theta = 2 * PI * randf(); */    

	/* float x = radius * sinPhi * std::sinf(theta); */
	/* float y = radius * cosPhi; */
	/* float z = radius * sinPhi * std::cosf(theta); */
	

	float r1 = drand48();
	float r2 = drand48();
	float phi = 2*M_PI*r1;
	float x = cos(phi)*sqrt(r2);
	float y = sin(phi)*sqrt(r2);
	float z = sqrt(1-r2);
	return Eigen::Vector3f(x, y, z).normalized();

	// TODO
	/* float phi = 2 * M_PI * drand48(); */
	/* float theta = arccos(2 * drand48() - 1); */

	/* float x = r * sin(theta) * cos(phi); */
	/* float y = r * sin(theta) * sin(phi); */
	/* float z = r * cos(theta); */
	/* Eigen::Vector3f p = Vector3f(x, y, z); */
	/* return p; */



	/* // Sample between 0 and 1 for z direction */
	/* float eps1 = drand48(); */
	/* float eps2 = drand48(); */

	/* // Angle phi can lie anywhere on a circle */
	/* float r = drand48(); */
	/* float theta = 0; */
	/* float phi = 2 * M_PI * drand48(); */

	/* // Compute radius = sin(theta) */
	/* double xy_rad = sqrt(1 - r * r); */

	/* p[0] = r * cos(phi); */
	/* p[1] = r * sin(phi); */
	/* p[2] = xy_rad; */
	/* return p; */
}

#endif
