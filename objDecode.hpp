#include <vector>
#include <cstdlib>

class Vertex {
public:
	float x; float y; float z;

	void print() { std::cout << "Vertex: ( " << x << " , " << y << " , " << z << " )" << std::endl; }
};

class VertexTexture {
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
};

class Face {
public:
	std::vector<Vertex> v;
};

class OBJ {
public:
	std::vector<Vertex> v; 
	std::vector<VertexTexture> vt;
	std::vector<VertexNormal> vn;
	std::vector<VertexParameter> vp;
	std::vector<Face> faces;

	static OBJ* decodeObj(std::string filename);
	void printVertices();
};
