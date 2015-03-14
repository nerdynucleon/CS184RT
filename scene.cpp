#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif
#ifndef SCENE_H
#include "scene.hpp"
#endif
#ifndef VECTOR_H
#include <vector>
#define VECTOR_H
#endif

void Scene::add(Light *l){
  lights.push_back(l);
}

void Scene::add(sceneObject *obj){
  objects.push_back(obj);
}