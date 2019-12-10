#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#include <math.h>
#include <Eigen/Dense>

Eigen::Vector3f change_basis(Eigen::Vector3f dir, Eigen::Vector3f norm)
{
	// Construct basis vectors from normal vector
	Eigen::Vector3f w = norm;

	// Random vector direction
	Eigen::Vector3f u;
	if (fabs(w[0]) > 0.9)
		u = Eigen::Vector3f(0, 1, 0);
	else
		u = Eigen::Vector3f(1, 0, 0);

	Eigen::Vector3f v = w.cross(u).normalized();
	u = w.cross(v);


	// Change basis	
	Eigen::Vector3f new_dir;
	float x = dir[0], y = dir[1], z = dir[2];
	new_dir[0] = x * u[0] + y * v[0] + z * w[0];
	new_dir[1] = x * u[1] + y * v[1] + z * w[1];
	new_dir[2] = x * u[2] + y * v[2] + z * w[2];
	return new_dir;
}


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
	float r1 = drand48();
	float r2 = drand48();
	float phi = 2*M_PI*r1;
	float x = cos(phi)*sqrt(r2);
	float y = sin(phi)*sqrt(r2);
	float z = sqrt(1-r2);
	return Eigen::Vector3f(x, y, z).normalized();
}

#endif
