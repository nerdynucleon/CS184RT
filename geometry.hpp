#ifndef GEOMETRY_H
#define GEOMETRY_H

#ifndef COLOR_H
#include "color.hpp"
#endif
#include <cstdlib>
#include <iostream>

/* Used as a point in space or as a direction */
class vec3{
  public:
    vec3(float x, float y, float z);
    vec3();
    vec3 operator+(vec3);
    vec3 operator*(float);
    float operator*(vec3);
    vec3 operator-(vec3);
    vec3 operator-();
    float x; float y; float z;

    /* Debug */
    void print() { std::cout << "vec3: ( " << x << " , " << y << " , " << z << " )" << std::endl; }
};

/* Positional Light */
class Light{
  public:
    vec3 pos;
    RGB intensity;
    Light(float, float, float, float, float, float);
    Light(RGB*, float, float, float);
};

/* Returned from a ray object intersection*/
class diffGeom{
  public:
    vec3 pos;
    vec3 normal;
    BRDF *brdf;
};

/* vector defined by the start and end locations */
class ray{
  public:
    ray(vec3, vec3, float, float);
    vec3 pos;
    vec3 dir;
    float t_min;
    float t_max;
};

/* Parent class for scene geoemetry */
class sceneObject{
  public:
    virtual diffGeom *intersect(ray) = 0;
};

/* Geometry Primitive */
class triangle: public sceneObject {
  vec3 *v1; vec3 *v2; vec3 *v3;
  vec3 *n1; vec3 *n2; vec3 *n3;
  BRDF *brdf;
  public:
    diffGeom *intersect(ray);
    triangle(vec3,vec3,vec3,BRDF*);
    triangle(vec3,vec3,vec3,vec3,vec3,vec3,BRDF*);
};
class sphere: public diffGeom{

  public:
    sphere(float, float, float, float);
    sphere(vec3, float);
    diffGeom *intersect(ray);
    vec3 center;
    float radius;
};

#endif