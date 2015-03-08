#ifndef GEOMETRY_H
#define GEOMETRY_H

#ifndef COLOR_H
#include "color.hpp"
#endif

/* Used as a point in space or as a direction */
class vec3{
  public:
    vec3(float x, float y, float z);
    vec3();
    vec3 operator+(vec3);
    vec3 operator*(float);
    vec3 operator-(vec3);
    float x; float y; float z;
};

class sphere{};

class Light{
  vec3 pos;
  RGB intensity;
  public:
    Light(float, float, float, float, float, float);
    Light(RGB*, float, float, float);
};

class diffGeom{
  vec3 pos;
  vec3 normal;
  BRDF *brdf;
};

/* vector defined by the start and end locations */
class ray{
  vec3 pos;
  vec3 dir;
  float t_min;
  float t_max;
  public:
    ray(vec3, vec3, float, float);
};

/* Parent class for scene geoemetry */
class sceneObject{
  public:
    diffGeom *intersect(ray);
};

class triangle{
  vec3 *v1; vec3 *v2; vec3 *v3;
  vec3 *n1; vec3 *n2; vec3 *n3;
  BRDF *brdf;
  public:
    triangle(vec3,vec3,vec3,BRDF*);
    triangle(vec3,vec3,vec3,vec3,vec3,vec3,BRDF*);
};
#endif