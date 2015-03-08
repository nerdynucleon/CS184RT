#include "geometry.hpp"

vec3::vec3(float xin, float yin, float zin){
  x = xin;
  y = yin;
  z = zin;
}

ray::ray(float x1, float x2, float y1, float y2, float z1, float z2):
  v1(x1, y1, z1), v2(x2, y2, z2){
  
}