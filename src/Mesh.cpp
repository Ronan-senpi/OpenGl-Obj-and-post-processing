//
// Created by Ronan on 12/01/2021.
//

#include "Mesh.h"
#include <cstdint>

Mesh::Mesh() {

	_objectColor = glm::vec3(1.0f, 0.5f, 0.31f);

	glGenVertexArrays(1, &_vao); //Generate VAO BUFFER
	glBindVertexArray(_vao); // Bind current vao

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0); //Set parameters 0 format
	glEnableVertexAttribArray(0); // Start
	glVertexAttribBinding(0, 0);// take data form buffer 0


	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat)); //Set parameters 1 format
	glEnableVertexAttribArray(1); // Start
	glVertexAttribBinding(1, 0);// take data form buffer 0

	glBindVertexArray(0); //Unbind VAO


}

void Mesh::setVertices(const float *vertices, size_t size) {

	glGenBuffers(1, &_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(_vao);
	glBindVertexBuffer(0, _vbo, 0, 8 * sizeof(GLfloat));
	glBindVertexArray(0);
}

void Mesh::setIndices(const uint16_t *indices, size_t size) {

	glGenBuffers(1, &_ibo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint16_t), indices, GL_STATIC_DRAW);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBindVertexArray(0);
}

void Mesh::unbind() {
	glBindVertexArray(0);
}

void Mesh::bind() {
	glBindVertexArray(_vao);
}


