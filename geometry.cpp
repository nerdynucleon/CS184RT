#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif
#ifndef COLOR_H
#include "color.hpp"
#endif
#ifndef INPUT_H
#include "input.hpp"
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
diffGeom::diffGeom(vec3 p, vec3 n, BRDF *in, float tin){
  pos = p;
  normal = n;
  brdf = in;
  t = tin;
}

/* Construct a ray */
ray::ray(vec3 pin, vec3 din, float t_minimum, float t_maximum){
  pos = pin;
  dir = normalize(din);
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

bool checkIntersection(ray *r, float t_max, float t_calculated){
  return (t_calculated < t_max) && (t_calculated < r->t_max) && (t_calculated > r->t_min);
}

/* Ray triangle intersection */
bool triangle::intersect(ray r, diffGeom* dg, float t_max){
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
  if(v < 0.0f || u + v  > 1.0f) return 0;
  float t = (e2*q) * inv_det; 
  if(checkIntersection(&r,t_max,t)) { //ray intersection
    /* Interpolate Triangle Normals
    IMPLEMENT LATER */
    *dg = diffGeom(r.pos + r.dir*t, normalize((*n2)*u + (*n1)*(1-u-v) + (*n3)*v), brdf, t);
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
  return (*this) - n*((*this)*2.0f*n/(pow(n.length(),2)));
}

sphere::sphere(float x, float y, float z, float radiusIn){
  radius = radiusIn;
  center = new vec3(x,y,z);
}

sphere::sphere(vec3 *v1, float radiusIn){
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
  n1 = norm2pt(cross(*v2in - *v1in , *v3in - *v1in)); n2 = n1; n3 = n1;
}

triangle::triangle(vec3* v1in,vec3* v2in,vec3* v3in,vec3* n1in,vec3* n2in,vec3* n3in,BRDF* brdfin){
  v1 = v1in; v2 = v2in; v3 = v3in; brdf = brdfin;
  n1 = n1in; n2 = n2in; n3 = n3in;
}

void sphere::print() {
  std::cout << "Sphere (" << this << "):" << std::endl;
  std::cout << "    " << "Center: "; center->print();
  std::cout << "    " << "Radius: " << radius << std::endl;
  std::cout << "    BRDF: " << brdf << std::endl;
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

bool sphere::intersect(ray r, diffGeom* dg, float t_max){
  if (transform) {
    r = ray(*(invT->transform(r.pos, true)), *(invT->transform(r.dir, false)), r.t_min, r.t_max);
  }
  float a = r.dir * r.dir;
  if(a == 0) return false;
  float b = r.dir*(r.pos - *center)*2;
  float c = (r.pos - *center)*(r.pos - *center) - radius*radius;
  float t1 = (-b - sqrt(b*b - 4*a*c))/(2*a);
  float t2 = (-b + sqrt(b*b - 4*a*c))/(2*a);
  if(checkIntersection(&r,t_max,t1)){
    vec3 tray = r.pos + r.dir*t1;
    if (transform) {
      tray = *(T->transform(tray, true));
    }
    //*dg = diffGeom(tray, normalize(r.pos + r.dir*t1 - *center), brdf, t1);
    *dg = diffGeom(tray, normalize(r.pos + r.dir*t1  - *center), brdf, t1);
    return true;
  } else if (checkIntersection(&r,t_max,t2)) {
    vec3 tray = r.pos + r.dir*t2;
    if (transform) {
      tray = *(T->transform(tray, true));
    }
     //*dg = diffGeom(tray, normalize(r.pos + r.dir*t2 - *center), brdf, t2);
    *dg = diffGeom(tray, normalize(r.pos + r.dir*t2 - *center), brdf, t2);
    return true;
  }
  return false;
}

vec4::vec4() {
  x = 0; y = 0; z = 0; w = 0;
}

vec4::vec4(vec3 v1, bool pos){
x = v1.x; y = v1.y; z = v1.z;
  if(pos){
    w = 1; 
  }else{
    w = 0; 
  } 
}

vec4::vec4(float vx, float vy, float vz, float vw) {
  x = vx; y = vy; z = vz; w = vw;
}

/* Dot product */
float vec4::operator*(vec4 v2) {
  return v2.x * x + v2.y * y + v2.z * z + v2.w * w;
}

vec4 vec4::operator*(float f) {
  return vec4(x * f, y * f, z * f, w * f);
}

void vec4::print() {
  std::cout << x << " " << y << " " << z << " " << w << std::endl;
}

Matrix::Matrix(float a, float b, float c, float d, float e, float f, float g, float h,
               float i, float j, float k, float l, float m, float n, float o, float p) {
  v[0] = new vec4(a, b, c, d);
  v[1] = new vec4(e, f, g, h);
  v[2] = new vec4(i, j, k, l);
  v[3] = new vec4(m, n, o, p);
}

Matrix::Matrix(vec4 *a, vec4 *b, vec4 *c, vec4 *d) {
  v[0] = a; v[1] = b; v[2] = c; v[3] = d;
}

Matrix::Matrix() {
  v[0] = new vec4(1, 0, 0, 0);
  v[1] = new vec4(0, 1, 0, 0);
  v[2] = new vec4(0, 0, 1, 0);
  v[3] = new vec4(0, 0, 0, 1);
}

Matrix::Matrix(Matrix *copy) {
  v[0] = new vec4(copy->v[0]->x, copy->v[0]->y, copy->v[0]->z, copy->v[0]->w);
  v[1] = new vec4(copy->v[1]->x, copy->v[1]->y, copy->v[1]->z, copy->v[1]->w);
  v[2] = new vec4(copy->v[2]->x, copy->v[2]->y, copy->v[2]->z, copy->v[2]->w);
  v[3] = new vec4(copy->v[3]->x, copy->v[3]->y, copy->v[3]->z, copy->v[3]->w);
}

vec4* Matrix::column(int n) {
  if (n == 0) { return new vec4(v[0]->x, v[1]->x, v[2]->x, v[3]->x); }
  if (n == 1) { return new vec4(v[0]->y, v[1]->y, v[2]->y, v[3]->y); }
  if (n == 2) { return new vec4(v[0]->z, v[1]->z, v[2]->z, v[3]->z); }
  return new vec4(v[0]->w, v[1]->w, v[2]->w, v[3]->w);
}

vec3* Matrix::operator*(vec4 vin){
  float w =(*v[3])*vin;
  float x; float y; float z;
  if(w == 0){
    x = (*v[0])*vin;
    y = (*v[1])*vin;
    z = (*v[2])*vin;
  } else {
    x = (*v[0])*vin /w;
    y = (*v[1])*vin /w;
    z = (*v[2])*vin /w;
  }
  return new vec3(x,y,z);
}

vec3* Matrix::transform(vec3 vin, bool pos){
  vec4 out = vec4(vin,pos);
  return (*this)*out;
}

void Matrix::print() {
  std::cout << "Matrix (" << this << "): " << std::endl;
  std::cout << "    "; v[0]->print();
  std::cout << "    "; v[1]->print();
  std::cout << "    "; v[2]->print();
  std::cout << "    "; v[3]->print();
}

Matrix* Matrix::operator*(Matrix m2) {
  Matrix *result = new Matrix();
  vec4 *c[4];
  c[0] = m2.column(0); c[1] = m2.column(1); c[2] = m2.column(2); c[3] = m2.column(3);
  for (int i = 0; i < 4; i++) {
    result->v[i]->x = *v[i] * (*c[0]);
    result->v[i]->y = *v[i] * (*c[1]);
    result->v[i]->z = *v[i] * (*c[2]);
    result->v[i]->w = *v[i] * (*c[3]);
  }
  delete c[0]; delete c[1]; delete c[2]; delete c[3];
  return result;
}

Matrix* Matrix::operator*(float scalar) {
  float d[16]; int j = 0;
  for (int i = 0; i < 4; i++) {
    d[j] = v[i]->x * scalar;
    d[j+1] = v[i]->y * scalar;
    d[j+2] = v[i]->z * scalar;
    d[j+3] = v[i]->w * scalar;
    j += 4;
  }
  return new Matrix(d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9], d[10],
    d[11], d[12], d[13], d[14], d[15]);
}

float Matrix::determinant() {
  float sum = 0;
  sum += v[0]->x * v[1]->y * v[2]->z * v[3]->w;
  sum += v[0]->x * v[1]->z * v[2]->w * v[3]->y;
  sum += v[0]->x * v[1]->w * v[2]->y * v[3]->z;

  sum += v[0]->y * v[1]->x * v[2]->w * v[3]->z;
  sum += v[0]->y * v[1]->z * v[2]->x * v[3]->w;
  sum += v[0]->y * v[1]->w * v[2]->z * v[3]->x;

  sum += v[0]->z * v[1]->x * v[2]->y * v[3]->w;
  sum += v[0]->z * v[1]->y * v[2]->w * v[3]->x;
  sum += v[0]->z * v[1]->w * v[2]->x * v[3]->y;

  sum += v[0]->w * v[1]->x * v[2]->z * v[3]->y;
  sum += v[0]->w * v[1]->y * v[2]->x * v[3]->z;
  sum += v[0]->w * v[1]->z * v[2]->y * v[3]->x;

  sum -= v[0]->x * v[1]->y * v[2]->w * v[3]->z;
  sum -= v[0]->x * v[1]->z * v[2]->y * v[3]->w;
  sum -= v[0]->x * v[1]->w * v[2]->z * v[3]->y;

  sum -= v[0]->y * v[1]->x * v[2]->z * v[3]->w;
  sum -= v[0]->y * v[1]->z * v[2]->w * v[3]->x;
  sum -= v[0]->y * v[1]->w * v[2]->x * v[3]->z;

  sum -= v[0]->z * v[1]->x * v[2]->w * v[3]->y;
  sum -= v[0]->z * v[1]->y * v[2]->x * v[3]->w;
  sum -= v[0]->z * v[1]->w * v[2]->y * v[3]->x;

  sum -= v[0]->w * v[1]->x * v[2]->y * v[3]->z;
  sum -= v[0]->w * v[1]->y * v[2]->z * v[3]->x;
  sum -= v[0]->w * v[1]->z * v[2]->x * v[3]->y;

  return sum;
}

Matrix* Matrix::inverse() {
  float det = determinant();
  if (det == 0) {
    return this;
  }
  Matrix* result = new Matrix();
  result->v[0]->x = v[1]->y * v[2]->z * v[3]->w + 
                   v[1]->z * v[2]->w * v[3]->y +
                   v[1]->w * v[2]->y * v[3]->z -
                   v[1]->y * v[2]->w * v[3]->z -
                   v[1]->z * v[2]->y * v[3]->w -
                   v[1]->w * v[2]->z * v[3]->y;

  result->v[0]->y = v[0]->y * v[2]->w * v[3]->z +
                   v[0]->z * v[2]->y * v[3]->w +
                   v[0]->w * v[2]->z * v[3]->y -
                   v[0]->y * v[2]->z * v[3]->w -
                   v[0]->z * v[2]->w * v[3]->y -
                   v[0]->w * v[2]->y * v[3]->z;

  result->v[0]->z = v[0]->y * v[1]->z * v[3]->w +
                   v[0]->z * v[1]->w * v[3]->y +
                   v[0]->w * v[1]->y * v[3]->z -
                   v[0]->y * v[1]->w * v[3]->z -
                   v[0]->z * v[1]->y * v[3]->w -
                   v[0]->w * v[1]->z * v[3]->y;

  result->v[0]->w = v[0]->y * v[1]->w * v[2]->z +
                   v[0]->z * v[1]->y * v[2]->w +
                   v[0]->w * v[1]->z * v[2]->y -
                   v[0]->y * v[1]->z * v[2]->w -
                   v[0]->z * v[1]->w * v[2]->y -
                   v[0]->w * v[1]->y * v[2]->z;

  result->v[1]->x = v[1]->x * v[2]->w * v[3]->z +
                   v[1]->z * v[2]->x * v[3]->w +
                   v[1]->w * v[2]->z * v[3]->x -
                   v[1]->x * v[2]->z * v[3]->w -
                   v[1]->z * v[2]->w * v[3]->x -
                   v[1]->w * v[2]->x * v[3]->z;

  result->v[1]->y = v[0]->x * v[2]->z * v[3]->w +
                   v[0]->z * v[2]->w * v[3]->x +
                   v[0]->w * v[2]->x * v[3]->z - 
                   v[0]->x * v[2]->w * v[3]->z -
                   v[0]->z * v[2]->x * v[3]->w - 
                   v[0]->w * v[2]->z * v[3]->x;

  result->v[1]->z = v[0]->x * v[1]->w * v[3]->z +
                   v[0]->z * v[1]->x * v[3]->w +
                   v[0]->w * v[1]->z * v[3]->x -
                   v[0]->x * v[1]->z * v[3]->w -
                   v[0]->z * v[1]->w * v[3]->x -
                   v[0]->w * v[1]->x * v[3]->z;

  result->v[1]->w = v[0]->x * v[1]->z * v[2]->w +
                   v[0]->z * v[1]->w * v[2]->x +
                   v[0]->w * v[1]->x * v[2]->z - 
                   v[0]->x * v[1]->w * v[2]->z -
                   v[0]->z * v[1]->x * v[2]->w - 
                   v[0]->w * v[1]->z * v[2]->x;

  result->v[2]->x = v[1]->x * v[2]->y * v[3]->w +
                   v[1]->y * v[2]->w * v[3]->x +
                   v[1]->w * v[2]->x * v[3]->y -
                   v[1]->x * v[2]->w * v[3]->y -
                   v[1]->y * v[2]->x * v[3]->w -
                   v[1]->w * v[2]->y * v[3]->x;

  result->v[2]->y = v[0]->x * v[2]->w * v[3]->y +
                   v[0]->y * v[2]->x * v[3]->w +
                   v[0]->w * v[2]->y * v[3]->x -
                   v[0]->x * v[2]->y * v[3]->w -
                   v[0]->y * v[2]->w * v[3]->x -
                   v[0]->w * v[2]->x * v[3]->y;

  result->v[2]->z = v[0]->x * v[1]->y * v[3]->w +
                   v[0]->y * v[1]->w * v[3]->x +
                   v[0]->w * v[1]->x * v[3]->y -
                   v[0]->x * v[1]->w * v[3]->y -
                   v[0]->y * v[1]->x * v[3]->w -
                   v[0]->w * v[1]->y * v[3]->x;

  result->v[2]->w = v[0]->x * v[1]->w * v[2]->y +
                   v[0]->y * v[1]->x * v[2]->w +
                   v[0]->w * v[1]->y * v[2]->x -
                   v[0]->x * v[1]->y * v[2]->w -
                   v[0]->y * v[1]->w * v[2]->x -
                   v[0]->w * v[1]->x * v[2]->y;

  result->v[3]->x = v[1]->x * v[2]->z * v[3]->y +
                   v[1]->y * v[2]->x * v[3]->z +
                   v[1]->z * v[2]->y * v[3]->x -
                   v[1]->x * v[2]->y * v[3]->z -
                   v[1]->y * v[2]->z * v[3]->x -
                   v[1]->z * v[2]->x * v[3]->y;

  result->v[3]->y = v[0]->x * v[2]->y * v[3]->z +
                   v[0]->y * v[2]->z * v[3]->x +
                   v[0]->z * v[2]->x * v[3]->y -
                   v[0]->x * v[2]->z * v[3]->y -
                   v[0]->y * v[2]->x * v[3]->z -
                   v[0]->z * v[2]->y * v[3]->x;

  result->v[3]->z = v[0]->x * v[1]->z * v[3]->y +
                   v[0]->y * v[1]->x * v[3]->z +
                   v[0]->z * v[1]->y * v[3]->x -
                   v[0]->x * v[1]->y * v[3]->z -
                   v[0]->y * v[1]->z * v[3]->x -
                   v[0]->z * v[1]->x * v[3]->y;

  result->v[3]->w = v[0]->x * v[1]->y * v[2]->z +
                   v[0]->y * v[1]->z * v[2]->x +
                   v[0]->z * v[1]->x * v[2]->y -
                   v[0]->x * v[1]->z * v[2]->y -
                   v[0]->y * v[1]->x * v[2]->z -
                   v[0]->z * v[1]->y * v[2]->x;

  return (*result) * (1/det);
}
