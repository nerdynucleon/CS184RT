#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif
#ifndef COLOR_H
#include "color.hpp"
#endif

vec3 vec3::operator+(vec3 v2){
  return vec3(x + v2.x, y + v2.y, z + v2.z);
}

vec3 vec3::operator*(float scale){
  return vec3(x * scale, y * scale, z * scale);
}

vec3 vec3::operator-(vec3 v2){
  return vec3(x - v2.x, y - v2.y, z - v2.z);
}

vec3::vec3(float xin, float yin, float zin){
  x = xin; y = yin; z = zin;
}

ray::ray(vec3 pin, vec3 din, float t_minimum, float t_maximum){
  pos = pin;
  dir = din;
  t_min = t_minimum;
  t_max = t_maximum;
}

Light::Light(float rIn, float gIn, float bIn, float x, float y, float z):
  intensity(rIn, gIn, bIn), pos(x,y,z){
}

Light::Light(RGB *colorIn, float x, float y, float z):
  intensity(colorIn->r, colorIn->g, colorIn->b), pos(x,y,z){
}