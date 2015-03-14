#include <vector>
#include <cstdlib>
#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif

/*class vec3 {
public:
	int dimension;
	float x; float y; float z;

	void print() { std::cout << "vec3: ( " << x << " , " << y << " , " << z << " )" << std::endl; }
};*/

/* class Face {
public:
	int type;
	std::vector<vec3> v;
	std::vector<vec3> vt;
	std::vector<vec3> vn;
	void print() {
		std::cout << "Face: " << std::endl;
		for (int i = 0; i < v.size(); i++) {
			v[i].print();
		}
		for (int i = 0; i < vt.size(); i++) {
			vt[i].print();
		}
		for (int i = 0; i < vn.size(); i++) {
			vn[i].print();
		}
	};
}; */

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
