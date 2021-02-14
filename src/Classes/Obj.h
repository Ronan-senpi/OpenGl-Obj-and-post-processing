//
// Created by Ronan on 08/02/2021.
//

#ifndef PHONG_OBJ_H
#define PHONG_OBJ_H

#include "glm/glm.hpp"
#include <vector>
#include <string>
#include "tiny_obj_loader.h"
#include "VerticesStruct.h"

class Obj {
private:
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string path;
	std::vector<uint16_t> indices;
	std::vector<float> vertices;

	void setVertices();

public:
	Obj(const std::string &path, const std::string &filename);

	void loadObj(const std::string &path, const std::string &filename);

	inline std::vector<float> getVertices() {
		return vertices;
	}

	inline std::vector<uint16_t> getIndices() {
		return indices;
	}
	inline std::string getTextureName(){
		return  materials[0].diffuse_texname;
	}
};


#endif //PHONG_OBJ_H
