#ifndef INPUT_H
#define INPUT_H
#ifndef SCENE_H
#include "scene.hpp"
#endif

void parseInput(int argc, char** argv, Scene *s);
std::vector<Transformation*> xf;

#endif