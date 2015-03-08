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

// trim from start
inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}
/*****************/

bool startsWith(std::string a, std::string start) {
	return a.compare(0, start.length(), start) == 0;
}

/***/
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

void storeVertex(OBJ* decoded, std::vector<std::string> tokens) {
	if (tokens.size() <= 1) { return; }
	Vertex v;
	int j = 0;
	for (int i = 1; i < tokens.size(); i++) {
		if (tokens[i].compare(" ") == 0) { continue; }
		if (tokens[i].compare("#") == 0) { break; }
		if (j == 0) { v.x = std::stof(tokens[i]); j += 1; }
		else if (j == 1) { v.y = std::stof(tokens[i]); j += 1; }
		else if (j == 2) { v.z = std::stof(tokens[i]); j += 1; }
	}
	decoded->v.push_back(v);
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
			if (tokens[0].compare("v") == 0) {
				storeVertex(decoded, tokens);
			}
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

int main(int argc, char** argv){
	OBJ* abc = OBJ::decodeObj("sample/cube.obj");
	abc->printVertices();
	return 0;
}