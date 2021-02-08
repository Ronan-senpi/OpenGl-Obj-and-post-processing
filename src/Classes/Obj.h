//
// Created by Ronan on 08/02/2021.
//

#ifndef PHONG_OBJ_H
#define PHONG_OBJ_H

#include "glm/glm.hpp"
#include <vector>

class Obj {
private:
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
public:
	Obj(const char * path,
	    std::vector < glm::vec3 > & out_vertices,
	    std::vector < glm::vec2 > & out_uvs,
	    std::vector < glm::vec3 > & out_normals);

};


#endif //PHONG_OBJ_H
