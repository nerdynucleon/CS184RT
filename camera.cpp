#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif

#include "camera.hpp"
#include <cfloat>

camera::camera(float input[]):
  eye(input[0], input[1], input[2]),
  up(input[3], input[4], input[5]),
  right(input[6], input[7], input[8]),
  view(input[9], input[10], input[11]){
}

ray camera::getRay(float u, float v){
  vec3 pos = eye;
  vec3 dir = (view + eye) + up * v + right * u - eye;
  return ray(pos, dir, 1, FLT_MAX);
}
