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

/* Construct a ray */
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
    return NULL;
  }
  float t = -(f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c))/M;
  if(t < r.t_min || t > r.t_max) return NULL;
  float gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c))/M;
  if(gamma < 0 || gamma > 1) return NULL;
  float beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g))/M;
  if(beta < 0 || beta > 1) return NULL;

}

sphere::sphere(float x, float y, float z, float radiusIn){
  radius = radiusIn;
  center = vec3(x,y,z);
}

sphere::sphere(vec3 v1, float radiusIn){
  center = v1;
  radius = radiusIn;
}

diffGeom* sphere::intersect(ray r){
  if(r.dir * r.dir == 0) return NULL;
  float t1 = -r.dir*(r.pos-center);
  float t2 = t1;
  float det = sqrt(pow(r.dir*(r.pos-center),2) - r.dir*r.dir*pow((pos-center)*(pos-center) - radius*radius,2));
  t1 -= det;
  t2 += det;
  t2 /= r.dir*r.dir;
  t1 /= r.dir*r.dir;
  if(t1 > r.t_min){
     
  }


}