//
// Created by Ronan on 08/02/2021.
//

#include <iostream>
#include "Obj.h"
#include "../Helpers/ObjLoaderHelper.h"


void Obj::loadObj(const std::string &path) {
	if (ObjLoaderHelper::LoadObj(path, attrib, shapes, materials)) {
		std::cout << path << " loaded !!! " << std::endl;
	}
}

Obj::Obj(const std::string &path) {
	loadObj(path);
}



