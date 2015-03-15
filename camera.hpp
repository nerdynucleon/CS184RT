#ifndef CAMERA_H
#define CAMERA_H
#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif

/* Scene Primitive used to describe what to render */
class camera{
  vec3* eye;
  vec3* up;
  vec3* view;
  vec3* right;
  public:
    camera(float[]);
    ray getRay(float,float);
};
#endif