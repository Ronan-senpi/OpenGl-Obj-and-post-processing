//
// Created by Ronan on 12/01/2021.
//

#ifndef PHONG_MESH_H
#define PHONG_MESH_H

#include "glm/glm.hpp"
#include <cstdint>
#include <glad/glad.h>

class Mesh {
public:
	Mesh();

	void setVertices(const float *vertices, size_t size);
	void setIndices(const uint16_t *indices, size_t size);
	void bind();
	void unbind();

	inline glm::vec3 getColor() const{
		return  _objectColor;
	}

private:
	GLuint _vao; // Vertex array objet (setup attrib shader)
	GLuint _vbo; // Vertices array object Buffer
	GLuint _ibo; // Index buffer object (index VBO)
	glm::vec3 _objectColor;
};


#endif //PHONG_MESH_H
