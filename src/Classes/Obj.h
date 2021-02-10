//
// Created by Ronan on 08/02/2021.
//

#ifndef PHONG_OBJ_H
#define PHONG_OBJ_H

#include "glm/glm.hpp"
#include <vector>
#include <string>
#include "tiny_obj_loader.h"

class Obj {
private:
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string path;
public:
	Obj(const std::string& path);

	void loadObj(const std::string &path);

	inline std::vector<float> getVetices(){
		return attrib.vertices;
	}
	inline std::vector<uint16_t> getIndices(){
		std::vector<uint16_t> v;
		for (auto & shape : shapes) {
			for (auto & indice : shape.mesh.indices){
				v.push_back(indice.vertex_index);
			}
		}
		return v;
	}

};


#endif //PHONG_OBJ_H
