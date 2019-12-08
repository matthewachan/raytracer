#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <regex>

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
					min_corner[0] = std::min(pos[0], min_corner[0]);
					max_corner[0] = std::max(pos[0], max_corner[0]);
					min_corner[1] = std::min(pos[1], min_corner[1]);
					max_corner[1] = std::max(pos[1], max_corner[1]);
					min_corner[2] = std::min(pos[2], min_corner[2]);
					max_corner[2] = std::max(pos[2], max_corner[2]);
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
			for (int i = 0; i < size; ++i) {
				/* std::cout << "Triangle: " << triangles[3*i] << std::endl << triangles[3*i+1] << std::endl << triangles[3*i+2] << std::endl; */
				list[i] = new triangle(triangles[3*i], triangles[3*i+1], triangles[3*i+2], m);
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
		Eigen::Vector3f min_corner;
		Eigen::Vector3f max_corner;
		hittable **list;
		int size;
		material *mat;
};

#endif
