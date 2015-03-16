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

/* Divide by float */
vec3 vec3::operator/(float f){
  return vec3(x/f, y/f, z/f);
}

/* Vector Constructor */
vec3::vec3(float xin, float yin, float zin){
  x = xin; y = yin; z = zin;
}

void vec3::print(){
  printf("%f %f %f\n",x,y,z);
}

float vec3::length(){
  return sqrt(x*x + y*y + z*z);
}

/* Calculate the distance between two positions */
float dist(vec3 v1, vec3 v2){
  return sqrt(pow(v1.x - v2.x,2) + pow(v1.y - v2.y,2) + pow(v1.z - v2.z,2));
}

vec3 normalize(vec3 v1){
  float length = sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
  return vec3(v1.x/length, v1.y/length, v1.z/length);
}

vec3* norm2pt(vec3 v1){
  float length = sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
  return new vec3(v1.x/length, v1.y/length, v1.z/length);
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

void ray::print(){
  printf("Ray:\n t_min: %f\n t_max: %f\npos: ",t_min,t_max);
  pos.print();
  printf("dir: ");
  dir.print();
}

Light::Light(float x, float y, float z, float rIn, float gIn, float bIn, int typeIn, int falloffIn){
  intensity = new RGB(rIn, gIn, bIn);
  v = new vec3(x,y,z);
  falloff = falloffIn;
  type = typeIn;
}

void Light::print() {
  std::string prefix = "Point";
  if (type == 1) { prefix = "Directional"; }
  else if (type == 2) { prefix = "Ambient"; }
  std::cout << prefix << " Light (" << this << "): " << std::endl;
  std::cout << "    "; intensity->print(); 
  if (type == 0) {
    std::cout << "    Pos: "; v->print();
    std::cout << "    Falloff: " << falloff << std::endl;
  }
  else if (type == 1) {
    std::cout << "    Dir: "; v->print();
  }
}

/* Ray triangle intersection */
bool triangle::intersect(ray r, float *t_min,diffGeom* dg){
  vec3 e1 = *v2 - *v1;
  vec3 e2 = *v3 - *v1;
  vec3 p = cross(r.dir,e2);
  float det = e1 * p;
  /* check divide by zero condition */ 
  if(det > -0.000001 && det < 0.000001) return false;
  float inv_det = 1.0f/det;
  vec3 T = r.pos - (*v1);
  /* Test u parameter */
  float u = (T*p) * inv_det;
  if(u < 0.0 || u > 1.0) return 0;
  vec3 q =  cross(T, e1);
  float v = (r.dir*q) * inv_det;
  //The intersection lies outside of the triangle
  if(v < 0.f || u + v  > 1.f) return 0;
  float t = (e2*q) * inv_det; 
  if(t > 0.000001) { //ray intersection
    /* Interpolate Triangle Normals
    IMPLEMENT LATER */
    if(dg != NULL) *dg = diffGeom(r.pos + r.dir*t, normalize(cross(e1,e2)), brdf);
    return true;
  }
  return false;
}

void diffGeom::print(){
  printf("Differential Geometry:\n");
  printf("Point: "); pos.print();
  printf("Normal: "); normal.print();
  std::cout << "    BRDF: " << brdf << std::endl;
}

/* Get reflected vector around normal */
vec3 vec3::reflect(vec3 n){
  return (*this) - n*((*this)*2.0f*n/(pow(dist(n,n),2)));
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
  return vec3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

triangle::triangle(vec3* v1in,vec3* v2in,vec3* v3in,BRDF* brdfin){
  v1 = v1in; v2 = v2in; v3 = v3in; brdf = brdfin;
  n1 = norm2pt(cross(*v1in, *v2in)); n2 = NULL; n3 = NULL;
}

triangle::triangle(vec3* v1in,vec3* v2in,vec3* v3in,vec3* n1in,vec3* n2in,vec3* n3in,BRDF* brdfin){
  v1 = v1in; v2 = v2in; v3 = v3in; brdf = brdfin;
  n1 = n1in; n2 = n2in; n3 = n3in;
}

void triangle::print() {
  std::cout << "Triangle (" << this << "):" << std::endl;
  std::cout << "    "; v1->print();
  std::cout << "    "; v2->print(); 
  std::cout << "    "; v3->print();
  std::cout << "    "; n1->print();
  if (n2 != NULL) {
    std::cout << "    "; n2->print(); std::cout << "    "; n3->print();
  }
  std::cout << "    BRDF: " << brdf << std::endl;
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
    if(dg != NULL) *dg = diffGeom(r.pos + r.dir*t1, normalize(r.pos + r.dir*t1 - center), brdf);
    return true;
  } else if (t2 < (*t_min) && t2 > r.t_min && t2 < r.t_max) {
    *t_min = t2;
    if(dg != NULL) *dg = diffGeom(r.pos + r.dir*t2, normalize(r.pos + r.dir*t2 - center), brdf);
    return true;
  }
  return false;
}