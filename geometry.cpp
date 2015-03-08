#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif
#ifndef COLOR_H
#include "color.hpp"
#endif

vec3::vec3(float xin, float yin, float zin){
  x = xin; y = yin; z = zin;
}

ray::ray(float x1, float x2, float y1, float y2, float z1, float z2):
  v1(x1, y1, z1), v2(x2, y2, z2){
}
ray::ray(vec3 *v1in, vec3 *v2in):
  v1(v1in->x, v1in->y, v1in->z), v2(v2in->x, v2in->y, v2in->z){
}
Light::Light(float intensityIn, float x, float y, float z):
  color(rIn, gIn, bIn), pos(x,y,z){
}

Light::Light(RGB *colorIn, float x, float y, float z):
  color(colorIn->r, colorIn->g, colorIn->b), pos(x,y,z){
}