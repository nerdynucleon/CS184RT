#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif
#ifndef OBJ_DECODE_H
#define OBJ_DECODE_H
#include <vector>
#include <cstdlib>

class OBJ {
public:
	std::vector<vec3> v; 
	std::vector<vec3> vt;
	std::vector<vec3> vn;
	std::vector<vec3> vp;
	std::vector<triangle> faces;

	static OBJ* decodeObj(std::string filename);
	void printVertices();
	void printFaces();
};
#endif
