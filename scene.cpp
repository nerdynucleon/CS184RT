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
#include <cfloat>

void Scene::add(Light *l){
  lights.push_back(l);
}

void Scene::add(sceneObject *obj){
  objects.push_back(obj);
}

diffGeom* Scene::trace(ray r, diffGeom *dg){
  *dg = NULL;
  float t_max = FLT_MAX;
  diffGeom* temp;
  for(int i = 0; i < objects.size(); i++){
    sceneObject* o = objects[i];
    temp=(*o).trace(r, &t_max);
    if(temp){
      *dg = temp; 
    }
 } 
}