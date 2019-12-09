#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <regex>
#include <cmath>

#include "triangle.hpp"
#include "material.hpp"

const std::regex V_FMT("[[:s:]]+[0-9]+[[:s:]]+[0-9]+[[:s:]]+[0-9]+.*");
const std::regex V_VN_FMT("[[:s:]]+"
			"[0-9]+//[0-9]+[[:s:]]+"
			"[0-9]+//[0-9]+[[:s:]]+"
			"[0-9]+//[0-9]+.*");
const std::regex V_VT_FMT("[[:s:]]+"
			"[0-9]+/[0-9]+/[[:s:]]+"
			"[0-9]+/[0-9]+/[[:s:]]+"
			"[0-9]+/[0-9]+/.*");
const std::regex V_VT_VN_FMT("[[:s:]]+"
			"[0-9]+/[0-9]+/[0-9]+[[:s:]]+"
			"[0-9]+/[0-9]+/[0-9]+[[:s:]]+"
			"[0-9]+/[0-9]+/[0-9]+.*");

class mesh : public hittable
{
	public:
		mesh(Eigen::Vector3f origin, const char *filepath, material *m) : mat(m)
		{
			int nfaces = 0;
			std::vector<Eigen::Vector3f> triangles;
			std::vector<Eigen::Vector3f> tmp_pos;
			printf("Loading OBJ file [%s]\n", filepath);

			FILE *file = fopen(filepath, "r");
			if (file == NULL) {
				printf("Failed to open the OBJ file.\n");
				getchar();
				return;
			}
			while (1) {
				char first_word[128];

				// Read the first word of the line
				if (fscanf(file, "%s", first_word) == EOF)
					break;

				if (strcmp(first_word, "v") == 0) {
					Eigen::Vector3f pos;
					fscanf(file, "%f %f %f\n", &pos[0], &pos[1], &pos[2]);
					pos[0] += origin[0];
					pos[1] += origin[1];
					pos[2] += origin[2];
					tmp_pos.push_back(pos);
				} else if (strcmp(first_word, "f") == 0) {

					unsigned int pos_idxs[3] = {0};
					unsigned int uv_idxs[3] = {0};
					unsigned int norm_idxs[3] = {0};

					char *line = new char[100];
					fscanf(file, "%[^\n]\n", line);

					if (std::regex_match(line, V_FMT))
						sscanf(line, "%d %d %d\n",
								&pos_idxs[0], &pos_idxs[1], &pos_idxs[2]
						      );
					else if (std::regex_match(line, V_VN_FMT))
						sscanf(line,
								"%d//%d %d//%d %d//%d\n",
								&pos_idxs[0], &norm_idxs[0],
								&pos_idxs[1], &norm_idxs[1],
								&pos_idxs[2], &norm_idxs[2]);
					else if (std::regex_match(line, V_VT_FMT))
						sscanf(line,
								"%d/%d/ %d/%d/ %d/%d/\n",
								&pos_idxs[0], &uv_idxs[0],
								&pos_idxs[1], &uv_idxs[1],
								&pos_idxs[2], &uv_idxs[2]);
					else if (std::regex_match(line, V_VT_VN_FMT))
						sscanf(line,
								"%d/%d/%d %d/%d/%d %d/%d/%d\n",
								&pos_idxs[0], &uv_idxs[0], &norm_idxs[0],
								&pos_idxs[1], &uv_idxs[1], &norm_idxs[1],
								&pos_idxs[2], &uv_idxs[2], &norm_idxs[2]
						      );
					else {
						printf("Failed to parse OBJ file.\
								Check OBJ format for validity.\n");
						fclose(file);
						return;
					}

					// Zero-based numbering
					--pos_idxs[0];
					--pos_idxs[1];
					--pos_idxs[2];

					triangles.push_back(tmp_pos[pos_idxs[0]]);
					triangles.push_back(tmp_pos[pos_idxs[1]]);
					triangles.push_back(tmp_pos[pos_idxs[2]]);

					++nfaces;

				} else {
					char buf[1000];
					fgets(buf, 1000, file);
				}
			}

			size = triangles.size() / 3;
			list = new hittable*[size];

			areas = new float[size];
			float smallest = INFINITY;

			for (int i = 0; i < size; ++i) {
				// Add new triangle to list
				list[i] = new triangle(triangles[3*i], triangles[3*i+1], triangles[3*i+2], m);

				// Compute area of the triangle
				Eigen::Vector3f cross = (triangles[3*i+1]-triangles[3*i]).cross(triangles[3*i+2]-triangles[3*i]);
				areas[i] = 0.5*cross.norm();
				if (areas[i] < smallest)
					smallest = areas[i];
			}
			// Divide areas of all triangles by the smallest triangle area
			int total = 0;
			for (int i = 0; i < size; ++i) {
				areas[i] = round(areas[i]/smallest);
				total += areas[i];
			}
			// Construct array of triangle indices with frequency proportional to its area
			prob = new int[total];
			total_prob = total;
			int cntr = 0;
			for (int i = 0; i < size; ++i) {
				int count = areas[i];	
				while(cntr < cntr + count)
					prob[cntr++] = i;
			}

			fclose(file);
		}
		virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const
		{
			hit_record temp;	
			bool hit_anything = false;
			double closest_so_far = tmax;
			for (int i = 0; i < size; ++i) {
				if (list[i]->hit(r, tmin, closest_so_far, temp)) {
					hit_anything = true;
					closest_so_far = temp.t;
					rec = temp;
				}
			}
			return hit_anything;
		}
		Eigen::Vector3f random_sample(Eigen::Vector3f origin, float &pdf)
		{
			int random_ind = (int)drand48()*total_prob;
			int random_triangle = prob[random_ind];
			pdf = total_prob / areas[random_triangle];
			
			triangle *tri = (triangle*) list[random_triangle];
			
			// Random sample on triangle
			float u, v;
			do {
				u = drand48();
				v = drand48();
			} while (u + v > 1);


			return u*tri->v0 + v*tri->v1 + (1.0f-(u+v))*tri->v2;
		}

		virtual bool bounding_box(aabb& box) const {

			// Check if first bounding box exists
			aabb temp_box;
			if (!list[0]->bounding_box(temp_box))
				return false;
			box = temp_box;


			// Group boxes together one-by-one
			for (int i = 1; i < size; ++i) {
				if (list[i]->bounding_box(temp_box))
					surrounding_box(temp_box, box);
				else
					return false;
			}
			return true;
		}
		hittable **list;
		int size;
		material *mat;
		int *prob;
		int total_prob;
		float *areas;
};

#endif
