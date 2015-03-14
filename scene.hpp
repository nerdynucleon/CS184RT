#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif

#define SCENE_H
#ifndef VECTOR_H
#include <vector>
#define VECTOR_H
#endif
#ifndef CAMERA_H
#include "camera.hpp"
#endif

class Scene{
  std::vector<sceneObject*> objects;
  std::vector<Light*> lights;
  public:
    Scene();
    void trace(ray, diffGeom*);
    void add(sceneObject*);
    void add(Light*);
    camera cam;
};