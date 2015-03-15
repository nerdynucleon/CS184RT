#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif
#ifndef COLOR_H
#include "color.hpp"
#endif

#ifndef NULL
#define NULL 0
#endif

#include <cmath>

/* Basic operations to perform on Vectors */
vec3::vec3(){
  x = 0; y = 0; z = 0;
}

/* Add two vectors to one another */
vec3 vec3::operator+(vec3 v2){
  return vec3(x + v2.x, y + v2.y, z + v2.z);
}

/* Negation operator */
vec3 vec3::operator-(){
  return vec3(-x, -y, -z);
}

/* Scale vector by a scalar */
vec3 vec3::operator*(float scale){
  return vec3(x * scale, y * scale, z * scale);
}

/* Dot product of two vectors */
float vec3::operator*(vec3 v2){
  return v2.x * x + v2.y * y + v2.z * z;
}

/* Subtract two vectors from one another */
vec3 vec3::operator-(vec3 v2){
  return vec3(x - v2.x, y - v2.y, z - v2.z);
}

/* Vector Constructor */
vec3::vec3(float xin, float yin, float zin){
  x = xin; y = yin; z = zin;
}

/* Calculate the distance between two positions */
float dist(vec3 v1, vec3 v2){
  return sqrt(pow(v1.x - v2.x,2) + pow(v1.y - v2.y,2) + pow(v1.z - v2.z,2));
}

vec3 norm2(vec3 v1){
  float length = sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
  return vec3(v1.x/length, v1.y/length, v1.z/length);
}

/* Construct differential geometry */
diffGeom::diffGeom(vec3 p, vec3 n, BRDF *in){
  pos = p;
  normal = n;
  brdf = in;
}

/* Construct a ray */
ray::ray(vec3 pin, vec3 din, float t_minimum, float t_maximum){
  pos = pin;
  dir = din;
  t_min = t_minimum;
  t_max = t_maximum;
}

Light::Light(float rIn, float gIn, float bIn, float x, float y, float z):
  intensity(rIn, gIn, bIn), v(x,y,z){
}

Light::Light(RGB *colorIn, float x, float y, float z):
  intensity(colorIn->r, colorIn->g, colorIn->b), v(x,y,z){
}

/* Ray triangle intersection */
bool triangle::intersect(ray r, float *t_min,diffGeom* dg){
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
  /* check divide by zero condition */ 
  if(M == 0.0){
    return false;
  }
  float t = -(f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c))/M;
  if(t > (*t_min)){
    return false;
  }
  if(t < r.t_min || t > r.t_max) return false;
  float gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c))/M;
  if(gamma < 0 || gamma > 1) return false;
  float beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g))/M;
  if(beta < 0 || beta > 1) return false;
  /* Interpolate Triangle Normals */
  vec3 norm = (*v1)*(1 - gamma - beta) + (*v2)*beta + (*v3)*gamma;
  *t_min = t;
  *dg = diffGeom(r.pos + r.dir*t, norm2(norm), brdf);
  return true;
}

sphere::sphere(float x, float y, float z, float radiusIn){
  radius = radiusIn;
  center = vec3(x,y,z);
}

sphere::sphere(vec3 v1, float radiusIn){
  center = v1;
  radius = radiusIn;
}

diffGeom::diffGeom(){ 
}

/* Cross Product of two vectors */
vec3 cross(vec3 v1,vec3 v2){
  
}

triangle::triangle(vec3* v1in,vec3* v2in,vec3* v3in,BRDF* brdfin){
  v1 = v1in; v2 = v2in; v3 = v3in; brdf = brdfin;
  n1 = NULL; n2 = NULL; n3 = NULL;
}

triangle::triangle(vec3* v1in,vec3* v2in,vec3* v3in,vec3* n1in,vec3* n2in,vec3* n3in,BRDF* brdfin){
  v1 = v1in; v2 = v2in; v3 = v3in; brdf = brdfin;
  n1 = n1in; n2 = n2in; n3 = n3in;
}

bool sphere::intersect(ray r, float *t_min, diffGeom* dg){
  if(r.dir * r.dir == 0) return false;
  float t1 = -r.dir*(r.pos-center);
  float t2 = t1;
  float det = sqrt(pow(r.dir*(r.pos-center),2) - r.dir*r.dir*pow((r.pos-center)*(r.pos-center) - radius*radius,2));
  t1 -= det;
  t2 += det;
  t2 /= r.dir*r.dir;
  t1 /= r.dir*r.dir;
  if(t1 < (*t_min) && t1 > r.t_min && t1 < r.t_max){
    *t_min = t1;
    *dg = diffGeom(r.pos + r.dir*t1, norm2(r.pos + r.dir*t1 - center), brdf);
    return true;
  } else if (t2 < (*t_min) && t2 > r.t_min && t2 < r.t_max) {
    *t_min = t2;
    *dg = diffGeom(r.pos + r.dir*t2, norm2(r.pos + r.dir*t2 - center), brdf);
    return true;
  }
  return false;
}