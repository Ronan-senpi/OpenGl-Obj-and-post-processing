//
// Created by Ronan on 08/02/2021.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Obj.h"
#include "../Helpers/ObjLoaderHelper.h"
#include "stb_image.h"

void Obj::loadObj(const std::string &path, const std::string &filename) {
	if (ObjLoaderHelper::LoadObj(path, filename, attrib, shapes, materials)) {
		std::cout << path << " loaded !!! " << std::endl;
		setVertices();
	}
}

Obj::Obj(const std::string &path, const std::string &filename) {
	loadObj(path, filename);
}

void Obj::setVertices() {

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				// Optional: vertex colors
				// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
				// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
				// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
				vertices.push_back(vx);
				vertices.push_back(vy);
				vertices.push_back(vz);
				vertices.push_back(nx);
				vertices.push_back(ny);
				vertices.push_back(nz);
				vertices.push_back(tx);
				vertices.push_back(ty);
			}
			index_offset += fv;
			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}
	for (int i = 0; i < vertices.size()/8; ++i) {
		indices.push_back(i);
	}

    //textureIndex = loadTexture(("resources/models/" + materials[0].diffuse_texname).c_str());

}




