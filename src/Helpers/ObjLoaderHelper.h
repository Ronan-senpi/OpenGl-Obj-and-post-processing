//
// Created by Ronan on 09/02/2021.
//
//
// g++ loader_example.cc
//

#ifndef PHONG_OBJLOADERHELPER_H
#define PHONG_OBJLOADERHELPER_H

#include "tiny_obj_loader.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>
#include <mmsystem.h>
#ifdef __cplusplus
}
#endif
#pragma comment(lib, "winmm.lib")
#else
#if defined(__unix__) || defined(__APPLE__)
#include <sys/time.h>
#else
#include <ctime>
#endif
#endif

#ifdef __clang__
#pragma clang diagnostic push
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#endif

class timerutil {
public:
#ifdef _WIN32
	typedef DWORD time_t;

	timerutil() { ::timeBeginPeriod(1); }

	~timerutil() { ::timeEndPeriod(1); }

	void start() { t_[0] = ::timeGetTime(); }

	void end() { t_[1] = ::timeGetTime(); }

	time_t sec() { return (time_t) ((t_[1] - t_[0]) / 1000); }

	time_t msec() { return (time_t) ((t_[1] - t_[0])); }

	time_t usec() { return (time_t) ((t_[1] - t_[0]) * 1000); }

	time_t current() { return ::timeGetTime(); }

#else
#if defined(__unix__) || defined(__APPLE__)
	typedef unsigned long int time_t;

	void start() { gettimeofday(tv + 0, &tz); }
	void end() { gettimeofday(tv + 1, &tz); }

	time_t sec() { return static_cast<time_t>(tv[1].tv_sec - tv[0].tv_sec); }
	time_t msec() {
	  return this->sec() * 1000 +
			 static_cast<time_t>((tv[1].tv_usec - tv[0].tv_usec) / 1000);
	}
	time_t usec() {
	  return this->sec() * 1000000 +
			 static_cast<time_t>(tv[1].tv_usec - tv[0].tv_usec);
	}
	time_t current() {
	  struct timeval t;
	  gettimeofday(&t, NULL);
	  return static_cast<time_t>(t.tv_sec * 1000 + t.tv_usec);
	}

#else  // C timer
	// using namespace std;
	typedef clock_t time_t;

	void start() { t_[0] = clock(); }
	void end() { t_[1] = clock(); }

	time_t sec() { return (time_t)((t_[1] - t_[0]) / CLOCKS_PER_SEC); }
	time_t msec() { return (time_t)((t_[1] - t_[0]) * 1000 / CLOCKS_PER_SEC); }
	time_t usec() { return (time_t)((t_[1] - t_[0]) * 1000000 / CLOCKS_PER_SEC); }
	time_t current() { return (time_t)clock(); }

#endif
#endif

private:
#ifdef _WIN32
	DWORD t_[2];
#else
#if defined(__unix__) || defined(__APPLE__)
	struct timeval tv[2];
	struct timezone tz;
#else
	time_t t_[2];
#endif
#endif
};

namespace ObjLoaderHelper {
	void PrintInfo(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes,
	               const std::vector<tinyobj::material_t> &materials, bool showDetails = true);

	bool LoadObj(const std::string &filename, tinyobj::attrib_t &attrib, std::vector<tinyobj::shape_t> &shapes,
	                 std::vector<tinyobj::material_t> &materials, bool triangulate = true);
};


#endif //PHONG_OBJLOADERHELPER_H
