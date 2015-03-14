#include <iostream>
#include <fstream>
#include "objDecode.hpp"

/* http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring */
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <string>
#include <sstream>

#define VERTEX_DEFAULT		0
#define VERTEX_NORMAL		1
#define VERTEX_TEXTURE		2
#define VERTEX_PARAMETER	3

#define FACE_V 		0
#define FACE_VVT 	1
#define FACE_VVTN 2
#define FACE_VN  	3

// trim from start
inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}
/*****************/


/* http://stackoverflow.com/questions/236129/split-a-string-in-c */
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
/***/

void storeVertex(OBJ* decoded, std::vector<std::string> tokens, int type) {
	if (tokens.size() <= 1) { return; }
	vec3 v;
	int dimension = 0;
	for (int i = 1; i < tokens.size(); i++) {
		if (tokens[i].compare(" ") == 0) { continue; }
		if ((tokens[i].compare("#") == 0) || (dimension == 3)) { break; }
		if (dimension == 0) { v.x = std::stof(tokens[i]); dimension += 1; }
		else if (dimension == 1) { v.y = std::stof(tokens[i]); dimension += 1; }
		else if (dimension == 2) { v.z = std::stof(tokens[i]); dimension += 1; }
	}
	if (type == VERTEX_DEFAULT) { decoded->v.push_back(v); }
	else if (type == VERTEX_NORMAL)  { decoded->vn.push_back(v); }
	else if (type == VERTEX_TEXTURE) { decoded->vt.push_back(v); }
	else if (type == VERTEX_PARAMETER) { decoded->vp.push_back(v); }
}

void storeFace(OBJ* decoded, std::vector<std::string> tokens) {
	if (tokens.size() <= 1) { return; }
	Face f;
	vec3 v;
	for (int i = 1; i < tokens.size(); i++) {
		if (tokens[i].compare(" ") == 0) { continue; }
		if (tokens[i].compare("#") == 0) { break; }
		size_t find = tokens[i].find('/');
		if (find == std::string::npos) {
			v = decoded->v[decoded->v.size() - std::stoi(tokens[i])];
			f.v.push_back(v);
			f.type = FACE_V;
		}
		else if ((find != std::string::npos) && (tokens[i].substr(find, find+1).compare("/") == 0)) {
			std::vector<std::string> subtokens = split(tokens[i], char(47));
			v = decoded->v[decoded->v.size() - std::stoi(subtokens[0])];
			f.v.push_back(v);
			f.type = FACE_VN;
			v = decoded->vt[decoded->v.size() - std::stoi(subtokens[2])];
			f.vn.push_back(v);
		}
		else if (find != std::string::npos) {
			std::vector<std::string> subtokens = split(tokens[i], char(47));
			v = decoded->v[decoded->v.size() - std::stoi(subtokens[0])];
			f.v.push_back(v);
			v.print(); // debug
			f.type = FACE_V;
			if (subtokens.size() >= 2) {
				v = decoded->vt[decoded->v.size() - std::stoi(subtokens[1])];
				f.vt.push_back(v); f.type = FACE_VVT; 
			}
			if (subtokens.size() >= 3) { 
				v = decoded->vt[decoded->v.size() - std::stoi(subtokens[2])];
				f.vn.push_back(v); f.type = FACE_VVTN;
			}
		}
	}
	decoded->faces.push_back(f);
}


OBJ* OBJ::decodeObj(std::string filename){
	OBJ *decoded = new OBJ();
	std::string line;
	std::ifstream objIn(filename);
	if (objIn.is_open()) {
		while (getline(objIn, line)) {
			line = ltrim(line);
			std::vector<std::string> tokens = split(line, char(32));
			if (tokens.size() == 0) { continue; }
			else if (tokens[0].compare("v") == 0) { storeVertex(decoded, tokens, VERTEX_DEFAULT); }
			else if (tokens[0].compare("vt") == 0) { storeVertex(decoded, tokens, VERTEX_TEXTURE); }
			else if (tokens[0].compare("vn") == 0) { storeVertex(decoded, tokens, VERTEX_NORMAL); }
			else if (tokens[0].compare("vp") == 0) { storeVertex(decoded, tokens, VERTEX_PARAMETER); }
			else if (tokens[0].compare("f") == 0) { storeFace(decoded, tokens); }
		}
		objIn.close();
	}

	return decoded;
}

void OBJ::printVertices() {
	if (v.size() > 0) {
		for (int i = 0; i < v.size(); i++) {
			v[i].print();
		}
	}
}

void OBJ::printFaces() {
	if (faces.size() > 0) {
		for (int i = 0; i < faces.size(); i++) {
			faces[i].print();
		}
	}
}

int main(int argc, char** argv){
	OBJ* abc = OBJ::decodeObj("sample/sphere.obj");
	//abc->printVertices();
	abc->printFaces();
	return 0;
}