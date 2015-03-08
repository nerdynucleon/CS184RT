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

diffGeom* triangle::intersect(ray r){
  float a = v1->x - v2->x;
  float b = v1->y - v2->y;
  float c = v1->z - v2->z;
  float d = v1->x - v3->x;
  float e = v1->y - v3->y;
  float f = v1->z - v3->z;
  float g = r.dir.x;
  float h = r.dir.y;
  float i = r.dir.z;
  float j = v1->x - r.pos.x;
  float k = v1->y - r.pos.y;
  float l = v1->z - r.pos.z;
  float M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
  if(M == 0.0){
    return 0;
  }
  float t = -(f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c))/M;
  if(t < r.t_min || t > r.t_max) return 0;
  float gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c))/M;
  if(gamma < 0 || gamma > 1) return 0;
  float beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g))/M;
  if(beta < 0 || beta > 1) return 0;
  return 0;
}

diffGeom* sphere::intersect(ray r){

}