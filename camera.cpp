#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif

#ifndef CAMERA_H
#include "camera.hpp"
#define CAMERA_H
#endif
#ifndef CFLOAT_H
#include <cfloat>
#define CFLOAT_H
#endif

camera::camera(float input[]){
  vec3 lr = vec3(input[6], input[7], input[8]);
  vec3 ul = vec3(input[9],input[10],input[11]);
  eye = new vec3(input[0], input[1], input[2]);
  view = new vec3(eye->x - input[3], eye->y - input[4], eye->z - input[5]);
  up = new vec3(ul.x - eye->x, ul.y - eye->y, ul.z - eye->z);
  right = new vec3(lr.x - eye->x, lr.y - eye->y, lr.z - eye->z);
}

ray camera::getRay(float u, float v){
  vec3 pos = *eye;
  vec3 dir = ((*view) + (*eye)) + (*up) * v + (*right) * u - (*eye);
  return ray(pos, normalize(dir), dir.length(), FLT_MAX);
}

void camera::print(){
  printf("Camera:\n");
  printf("eye: %f %f %f\n",eye->x, eye->y, eye->z);
  printf("view: %f %f %f\n",view->x, view->y, view->z);
  printf("up: %f %f %f\n", up->x, up->y, up->z);
  printf("right: %f %f %f\n", right->x, right->y, right->z);
}