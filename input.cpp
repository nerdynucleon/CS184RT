#ifndef INPUT_H
#include "input.hpp"
#endif
#ifndef CAMERA_H
#include "camera.hpp"
#endif
#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif
#ifndef OBJDECODE_H
#include "objdecode.hpp"
#endif
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <iterator>
#include <cmath>


Matrix *transform = new Matrix();
bool applyTransform = false;

/* Splits a string by whitespace. */
std::vector<std::string> split(const std::string &str) {
	std::vector <std::string> tokens;
	std::stringstream stream(str);
	std::string temp;
	
	while (stream >> temp) {
		tokens.push_back(temp);
	}
	return tokens;
}

void argumentError(std::string command, int expected) {
	std::string err = "Too many arguments in \"" + command + "\". Expected: " + std::to_string(expected) + "\n";
	fprintf(stderr, "%s", err.c_str());
}

std::vector<float> parseLine(std::vector<std::string> tokens, int expected, std::string command) {
	std::vector<float> data;
	int depth = 0;
	for (std::vector<std::string>::size_type i = 1; i != tokens.size(); i++) {
		if (tokens[i].compare("") != 0) {
			if (depth == expected) { argumentError(command, expected); break; }
			data.push_back(std::stof(tokens[i]));
			depth += 1;
		}
	}
	return data;
}

void parseSphere(std::vector<std::string> tokens, Scene *s, BRDF *mat) {
	std::vector<float> data = parseLine(tokens, 4, "sph");
	/* Load sphere into scene */
	sphere* obj = new sphere(data[0], data[1], data[2], data[3]);
	obj->brdf = mat;
	s->add(obj);
	obj->transform = applyTransform;
	if (applyTransform) {
		obj->T = new Matrix(transform);
		obj->invT = transform->inverse();
	}
}

void parseCam(std::vector<std::string> tokens, Scene *s) {
	float data[15];
	int depth = 0;
	for (std::vector<std::string>::size_type i = 1; i != tokens.size(); i++) {
		if (tokens[i].compare("") != 0) {
			if (depth == 15) { argumentError("cam", 15); break; }
			data[depth] = std::stof(tokens[i]);
			depth += 1;
		}
	}
	s->cam = new camera(data);
}

void parsePointLight(std::vector<std::string> tokens, Scene *s) {
	std::vector<float> data = parseLine(tokens, 7, "ltp");
	Light *light = new Light(data[0], data[1], data[2], data[3], data[4], data[5], POINT, (int) data[6]);
	s->add(light);
}

void parseDirectional(std::vector<std::string> tokens, Scene *s) {
	std::vector<float> data = parseLine(tokens, 6, "ltd");
	Light *light = new Light(data[0], data[1], data[2], data[3], data[4], data[5], DIR, FALLOFF_NONE);
	s->add(light);
}

void parseAmbientLight(std::vector<std::string> tokens, Scene *s) {
	std::vector<float> data = parseLine(tokens, 3, "lta");
	Light *light = new Light(0, 0, 0, data[0], data[1], data[2], AMB, FALLOFF_NONE);
	s->add(light);
}

void parseTriangle(std::vector<std::string> tokens, Scene *s, BRDF *mat) {
	std::vector<float> data = parseLine(tokens, 9, "tri");
	/* Load tri into scene */
	vec3 v1 = vec3(data[0], data[1], data[2]);
	vec3 v2 = vec3(data[3], data[4], data[5]);
	vec3 v3 = vec3(data[6], data[7], data[8]);
	triangle *tri = new triangle(v1, v2, v3, mat);
	s->add(tri);
}



void parseObj(std::vector<std::string> tokens, Scene *s, BRDF *mat, Matrix m, bool b) {
	OBJ *obj = OBJ::decodeObj(tokens[1], s, mat, m, b);
}

void parseTranslation(std::vector<std::string> tokens) {
	/* Transformations should be applied to all OBJ 
	   unless reset by xfz */
	if(tokens.size() != 4 ){
		argumentError("xft", 9);
	}
	float tx = std::stof(tokens[1]); float ty = std::stof(tokens[2]); float tz = std::stof(tokens[3]);
	Matrix A = Matrix(1,0,0,tx, 0,1,0,ty, 0,0,1,tz, 0,0,0,1);
	*transform = (*transform) * A;
	applyTransform = true;
}

void parseRotation(std::vector<std::string> tokens) {
	/* Transformations should be applied to all OBJ 
	   unless reset by xfz */
	if(tokens.size() != 4 ){
		argumentError("xfr", 4);
	}
	float x = std::stof(tokens[1]); float y = std::stof(tokens[2]); float z = std::stof(tokens[3]);
	float length = sqrt(x*x + y*y + z*z); x /= length; y /= length; z /= length;
	if (length == 0) { return; }
	
	float angle = M_PI/180.0 *length;
	float sA = sin(angle);
	float cA = cos(angle);
	Matrix rx = Matrix(cA+x*x*(1-cA), x*y*(1-cA)-z*sA, y*sA+x*z*(1-cA), 0,
		z*sA+x*y*(1-cA), cA + y*y*(1-cA), -x*sA +y*z*(1-cA), 0,
		-y*sA+x*z*(1-cA), x*sA + y*z*(1-cA), cA + z*z*(1-cA), 0,
		0, 0, 0, 1);
	*transform = (*transform) * rx;
	applyTransform = true;
}

void parseScale(std::vector<std::string> tokens) {
	/* Transformations should be applied to all OBJ 
	   unless reset by xfz */
	if(tokens.size() != 4 ){
		argumentError("xfs", 4);
	}
	float x = std::stof(tokens[1]); float y = std::stof(tokens[2]); float z = std::stof(tokens[3]);
	Matrix A = Matrix(x,0,0,0, 0,y,0,0, 0,0,z,0, 0,0,0,1);
	*transform = (*transform) * A;
	applyTransform = true;
}

void parseReset(std::vector<std::string> tokens) {
	/* Reset Transformations */
	if(tokens.size() != 1){
		argumentError("xfz", 1);
	}
	applyTransform = false;
	*transform = Matrix();
}

BRDF* parseMat(std::vector<std::string> tokens, Scene *s) {
	float data[13]; int depth = 0;
	for (std::vector<std::string>::size_type i = 1; i != tokens.size(); i++) {
		if (depth == 13) { argumentError("mat", 13); break; }
		if (tokens[i].compare("") != 0) { 
			data[depth] = std::stof(tokens[i]);
			depth += 1;
		}
	}
	RGB *ka = new RGB(data[0], data[1], data[2]);
	RGB *kd = new RGB(data[3], data[4], data[5]);
	RGB *ks = new RGB(data[6], data[7], data[8]);
	RGB *kr = new RGB(data[10], data[11], data[12]);
	BRDF *mat = new BRDF(ka, kd, ks, kr, data[9]);
	return mat;
}


void parseInput(int argc, char** argv, Scene *s) {
	std::string line;
	BRDF *mat;
	while (getline(std::cin, line)) {
		std::vector<std::string> tokens = split(line);
		if (tokens.size() == 0) { continue; }
		if (tokens[0].compare("cam") == 0) { parseCam(tokens, s); }
		else if (tokens[0].compare("mat") == 0) { mat = parseMat(tokens, s); }
		else if (tokens[0].compare("sph") == 0) { parseSphere(tokens, s, mat); }
		else if (tokens[0].compare("ltp") == 0) { parsePointLight(tokens, s); }
		else if (tokens[0].compare("ltd") == 0) { parseDirectional(tokens, s); }
		else if (tokens[0].compare("lta") == 0) { parseAmbientLight(tokens, s); }
		else if (tokens[0].compare("tri") == 0) { parseTriangle(tokens, s, mat); }
		else if (tokens[0].compare("obj") == 0) { parseObj(tokens, s, mat, transform, applyTransform); }
		else if (tokens[0].compare("xft") == 0) { parseTranslation(tokens); }
		else if (tokens[0].compare("xfr") == 0) { parseRotation(tokens); }
		else if (tokens[0].compare("xfs") == 0) { parseScale(tokens); }
		else if (tokens[0].compare("xfz") == 0) { parseReset(tokens); }
	}
} 


