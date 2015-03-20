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

/* TODO: 
	• If a line has extra parameters then those parameters should be ignored and a warning message
should be printed to stderr
	• Any number of tabs and spaces is equivalent to a single space.
	• Lines with an unrecognized type should result in a warning message to stderr indicating an
unsupported feature and the program should then ignore the line. 
*/

std::vector<Transformation*> xf;

vec3* apply(vec3 *vin){
  if(xf.size() == 0){
    return vin;
  }
  Transformation *t;
  float a = vin->x; float b = vin->y; float c = vin->z;
  for(int i = 0; i < xf.size(); i++){
    t=xf[i];
    float x = t->x; float y = t->y; float z = t->z; float ct = t->ct; float st = t->st;
    if(t->type == TRANSLATE){
      /*Translate vector*/
      a = a + x;
      b = b + y;
      c = c + z;
    } else if(t->type == ROTATE){
      /* Exponential Map rotation in radians (length of rotation input) */
      float xout = a*(x*x +((y*y)+(z*z))*ct) + b*(y*x - x*y*ct -z*st) + c*(z*x - x*z*ct + y*st);
      float yout = a*(y*x -x*y*ct+z*st) + b*(y*y+(x*x+z*z)*ct) + c*(y*z-y*z*ct-x*st);
      float zout = a*(z*x -x*z*ct-y*st) + b*(z*y -y*z*ct+x*st) + c*(z*z +(x*x+y*y)*ct);
      a = xout;
      b = yout;
      c = zout;
    } else if(t->type == SCALE) {
      /* Scaling */
      a = a * x;
      b = b * y;
      c = c * z;
    }
  }
  free(vin);
  return new vec3(a,b,c);
}

vec3* apply(float a, float b, float c){
  Transformation *t;
  for(int i = 0; i < xf.size(); i++){
    t=xf[i];
    float x = t->x; float y = t->y; float z = t->z; float ct = t->ct; float st = t->st;
    if(t->type == TRANSLATE){
      /*Translate vector*/
      a = a + x;
      b = b + y;
      c = c + z;
    } else if(t->type == ROTATE){
      /* Exponential Map rotation in radians (length of rotation input) */
      float xout = a*(x*x +((y*y)+(z*z))*ct) + b*(y*x - x*y*ct -z*st) + c*(z*x - x*z*ct + y*st);
      float yout = a*(y*x -x*y*ct+z*st) + b*(y*y+(x*x+z*z)*ct) + c*(y*z-y*z*ct-x*st);
      float zout = a*(z*x -x*z*ct-y*st) + b*(z*y -y*z*ct+x*st) + c*(z*z +(x*x+y*y)*ct);
      a = xout;
      b = yout;
      c = zout;
    } else if(t->type == SCALE) {
      /* Scaling */
      a = a * x;
      b = b * y;
      c = c * z;
    }
  }
  return new vec3(a,b,c);
}

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
	obj->print();
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
	s->cam->print();
}

void parsePointLight(std::vector<std::string> tokens, Scene *s) {
	float data[7];
	int falloff = 0; int depth = 0;
	for (std::vector<std::string>::size_type i = 1; i != tokens.size(); i++) {
		if (tokens[i].compare("") != 0) { 
			if (depth == 8) { argumentError("ltp", 8); break; }
			if (depth != 7) {
				data[depth] = std::stof(tokens[i]);
			} else {
				falloff = std::stoi(tokens[i]);
			}
			depth += 1;
		}
	}
	Light *light = new Light(data[0], data[1], data[2], data[3], data[4], data[5], POINT, data[6]);
	light->print();
	s->add(light);
}

void parseDirectional(std::vector<std::string> tokens, Scene *s) {
	float data[6];
	int depth = 0;
	for (std::vector<std::string>::size_type i = 1; i != tokens.size(); i++) {
		if (tokens[i].compare("") != 0) { 
			if (depth == 6) { argumentError("ltd", 6); break; }
			data[depth] = std::stof(tokens[i]);
			depth += 1;
		}
	} 
	Light *light = new Light(data[0], data[1], data[2], data[3], data[4], data[5], DIR, FALLOFF_NONE);
	light->print();
	s->add(light);
}

void parseAmbientLight(std::vector<std::string> tokens, Scene *s) {
	float data[3];
	int depth = 0;
	for (std::vector<std::string>::size_type i = 1; i != tokens.size(); i++) {
		if (tokens[i].compare("") != 0) {
			if (depth == 3) { argumentError("lta", 3); break; } 
			data[depth] = std::stof(tokens[i]);
			depth += 1;
		}
	} 
	Light *light = new Light(0, 0, 0, data[0], data[1], data[2], AMB, FALLOFF_NONE);
	light->print();
	s->add(light);
}

void parseTriangle(std::vector<std::string> tokens, Scene *s, BRDF *mat) {
	float data[9];
	int depth = 0;
	for (std::vector<std::string>::size_type i = 1; i != tokens.size(); i++) {
		if (depth == 9) { argumentError("tri", 9); break; }
		if (tokens[i].compare("") != 0) { 
			data[depth] = std::stof(tokens[i]);
			depth += 1;
		}
	} 
	/* Load tri into scene */
	vec3 *v1 = new vec3(data[0], data[1], data[2]);
	vec3 *v2 = new vec3(data[3], data[4], data[5]);
	vec3 *v3 = new vec3(data[6], data[7], data[8]);
	triangle *tri = new triangle(v1, v2, v3, mat);
	s->add(tri);
	tri->print();
}


void parseObj(std::vector<std::string> tokens, Scene *s, BRDF *mat) {
	OBJ *obj = OBJ::decodeObj(tokens[1], s, mat);
}

void parseTranslation(std::vector<std::string> tokens) {
	/* Transformations should be applied to all OBJ 
	   unless reset by xfz */
	if(tokens.size() != 4 ){
		argumentError("xft", 9);
	}
	xf.push_back(new Transformation(std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3]),TRANSLATE));
}

void parseRotation(std::vector<std::string> tokens) {
	/* Transformations should be applied to all OBJ 
	   unless reset by xfz */
	if(tokens.size() != 4 ){
		argumentError("xfr", 4);
	}
	xf.push_back(new Transformation(std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3]),ROTATE));
}

void parseScale(std::vector<std::string> tokens) {
	/* Transformations should be applied to all OBJ 
	   unless reset by xfz */
	if(tokens.size() != 4 ){
		argumentError("xfs", 4);
	}
	xf.push_back(new Transformation(std::stof(tokens[1]),std::stof(tokens[2]),std::stof(tokens[3]),SCALE));
}

void parseReset(std::vector<std::string> tokens) {
	/* Reset Transformations */
	if(tokens.size() != 1){
		argumentError("xfz", 1);
	}
	while(!xf.empty()){
		free(xf.back());
		xf.pop_back();
	}
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
	mat->print(); 
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
		else if (tokens[0].compare("obj") == 0) { parseObj(tokens, s, mat); }
		else if (tokens[0].compare("xft") == 0) { parseTranslation(tokens); }
		else if (tokens[0].compare("xfr") == 0) { parseRotation(tokens); }
		else if (tokens[0].compare("xfs") == 0) { parseScale(tokens); }
		else if (tokens[0].compare("xfz") == 0) { parseReset(tokens); }
	}
} 
