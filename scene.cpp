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
#ifndef CAMERA_H
#include "camera.hpp"
#endif
#include <cfloat>


void Scene::add(Light *l){
  lights.push_back(l);
}

void Scene::add(sceneObject *obj){
  objects.push_back(obj);
}

bool Scene::trace(ray r, diffGeom *dg){
  float t_min = FLT_MAX;
  bool intersected = false;
  for(int i = 0; i < objects.size(); i++){
    if(objects[i]->intersect(r, dg, t_min)){
      t_min = dg->t;
      intersected = true;
    }
  }
  return intersected;
}