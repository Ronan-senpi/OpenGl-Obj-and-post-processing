//
// Created by Ronan on 08/02/2021.
//

#include <iostream>
#include "Obj.h"
#include "../Helpers/ObjLoaderHelper.h"


void Obj::loadObj(const std::string &filename, const std::string &path) {
	if (ObjLoaderHelper::LoadObj(filename, path, attrib, shapes, materials)) {
		std::cout << path << filename << " loaded !!! " << std::endl;
	}
}

Obj::Obj(const std::string &filename, const std::string &path) {
	loadObj(filename, path);
}



