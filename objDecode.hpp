#include <vector>
#include <cstdlib>

class Vertex {
public:
	int dimension;
	float x; float y; float z;

	void print() { std::cout << "Vertex: ( " << x << " , " << y << " , " << z << " )" << std::endl; }
};

/* class VertexTexture {
public:
	float u; float v; float w;
};

class VertexNormal {
public:
	float x; float y; float z;
};

class VertexParameter {
public:
	float u; float v; float w;
}; */

class Face {
public:
	int type;
	std::vector<Vertex> v;
	std::vector<Vertex> vt;
	std::vector<Vertex> vn;
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
};

class OBJ {
public:
	std::vector<Vertex> v; 
	std::vector<Vertex> vt;
	std::vector<Vertex> vn;
	std::vector<Vertex> vp;
	std::vector<Face> faces;

	static OBJ* decodeObj(std::string filename);
	void printVertices();
	void printFaces();
};
