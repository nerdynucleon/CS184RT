#ifndef INPUT_H
#include "input.hpp"
#endif
#ifndef CAMERA_H
#include "camera.hpp"
#endif
#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/* TODO: 
	• If a line has extra parameters then those parameters should be ignored and a warning message
should be printed to stderr
	• Any number of tabs and spaces is equivalent to a single space.
	• Lines with an unrecognized type should result in a warning message to stderr indicating an
unsupported feature and the program should then ignore the line. 
*/

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

void parseSphere(std::vector<std::string> tokens, Scene *s, BRDF *mat) {
	float data[4];
	int depth = 0;
	for (std::vector<std::string>::size_type i = 0; i != tokens.size(); i++) {
		if (tokens[i].compare(" ") != 0) { 
			data[i] = std::stof(tokens[i]);
			depth += 1;
		}
	}
	/* Load sphere into scene */
	sphere* obj = new sphere(data[0], data[1], data[2], data[3]);
	obj->brdf = mat;
	s->objects.push_back(obj);
}

void parseCam(std::vector<std::string> tokens, Scene *s) {
	float data[12];
	int depth = 0;
	for (std::vector<std::string>::size_type i = 0; i != tokens.size(); i++) {
		if (tokens[i].compare(" ") != 0) { 
			data[i] = std::stof(tokens[i]);
			depth += 1;
		}
	} 
	s->cam = new camera(data);
}

void parsePointLight(std::vector<std::string> tokens, Scene *s) {
	float data[6];
	int falloff = 0; int depth = 0;
	for (std::vector<std::string>::size_type i = 0; i != tokens.size(); i++) {
		if (tokens[i].compare(" ") != 0) { 
			if (depth != 6) {
				data[i] = std::stof(tokens[i]);
			} else {
				falloff = std::stoi(tokens[i]);
			}
			depth += 1;
		}
	} 
	/* TODO 
	lights.push_back(PointLight(data[0], data[1], data[2], data[3], data[4], data[5], falloff));
	*/
}

void parseDirectional(std::vector<std::string> tokens, Scene *s) {
	float data[6];
	int depth = 0;
	for (std::vector<std::string>::size_type i = 0; i != tokens.size(); i++) {
		if (tokens[i].compare(" ") != 0) { 
			data[i] = std::stof(tokens[i]);
			depth += 1;
		}
	} 
	/* TODO 
	lights.push_back(DirectionalLight(data[0], data[1], data[2], data[3], data[4], data[5]));
	*/
}

void parseAmbientLight(std::vector<std::string> tokens, Scene *s) {
	float data[3];
	int depth = 0;
	for (std::vector<std::string>::size_type i = 0; i != tokens.size(); i++) {
		if (tokens[i].compare(" ") != 0) { 
			data[i] = std::stof(tokens[i]);
			depth += 1;
		}
	} 
	/* TODO 
	lights.push_back(AmbientLight(data[0], data[1], data[2]));
	*/
}

void parseTriangle(std::vector<std::string> tokens, Scene *s, BRDF *mat) {
	float data[9];
	int depth = 0;
	for (std::vector<std::string>::size_type i = 0; i != tokens.size(); i++) {
		if (tokens[i].compare(" ") != 0) { 
			data[i] = std::stof(tokens[i]);
			depth += 1;
		}
	} 
	/* Load tri into scene */
	vec3* v1 = new vec3(data[0], data[1], data[2]);
	vec3* v2 = new vec3(data[3], data[4], data[5]);
	vec3* v3 = new vec3(data[6], data[7], data[8]);
	triangle* obj = new triangle(v1, v2, v3, mat);
	s->objects.push_back(obj);
}

/*
void parseObj(std::vector<std::string> tokens, Scene *s, BRDF *mat) {
	OBJ* obj = OBJ::decodeObj(tokens[1]);
	PUT OBJ DATA INTO SCENE
}*/

void parseTranslation(std::vector<std::string> tokens, Scene *s) {
	/* Transformations should be applied to all OBJ 
	   unless reset by xfz */
}

void parseRotation(std::vector<std::string> tokens, Scene *s) {
	/* Transformations should be applied to all OBJ 
	   unless reset by xfz */
}

void parseScale(std::vector<std::string> tokens, Scene *s) {
	/* Transformations should be applied to all OBJ 
	   unless reset by xfz */
}

void parseInput(int argc, char** argv, Scene *s) {
	std::string line;
	std::ifstream sceneIn(argv[1]);
	BRDF *mat;
	if (sceneIn.is_open()) {
		while (getline(sceneIn, line)) {
			std::vector<std::string> tokens = split(line, char(32));
			for (std::vector<std::string>::size_type i = 0; i != tokens.size(); i++) {
				if (tokens[0].compare("cam") == 0) { parseCam(tokens, s); }
				//else if (tokens[0].compare("mat") == 0) { mat = parseMat(tokens, s); }
				else if (tokens[0].compare("sph") == 0) { parseSphere(tokens, s, mat); }
				else if (tokens[0].compare("ltp") == 0) { parsePointLight(tokens, s); }
				else if (tokens[0].compare("ltd") == 0) { parseDirectional(tokens, s); }
				else if (tokens[0].compare("lta") == 0) { parseAmbientLight(tokens, s); }
				else if (tokens[0].compare("tri") == 0) { parseTriangle(tokens, s, mat); }
				//else if (tokens[0].compare("obj") == 0) { parseObj(tokens, s, mat); }
				//else if (tokens[0].compare("xft") == 0) { parseTranslation(tokens, s, mat); }
				//else if (tokens[0].compare("xfr") == 0) { parseRotation(tokens, s, mat); }
				//else if (tokens[0].compare("xfs") == 0) { parseScale(tokens, s, mat); }
				else if (tokens[0].compare("xft") == 0) { /* Set curr trans = identity */ }
			}
		}
		sceneIn.close();
	}
}

/* class Scene{
  public:
  	std::vector<sceneObject*> objects;
  	std::vector<Light*> lights;
    Scene();
    void trace(ray, diffGeom*);
    void add(sceneObject*);
    void add(Light*);
    camera cam;
}; */