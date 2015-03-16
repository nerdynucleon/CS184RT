#ifndef GEOMETRY_H
#define GEOMETRY_H

#ifndef COLOR_H
#include "color.hpp"
#endif
#include <cstdlib>

#define POINT 0
#define DIR 1
#define AMB 2

/* Falloff definitions (point lights only) */
#define FALLOFF_NONE    0
#define FALLOFF_LINEAR  1
#define FALLOFF_QUAD    2


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
    vec3 operator/(float);
    vec3 reflect(vec3);
    float x; float y; float z;
    void print();
    float length();
};

float dist(vec3, vec3);
vec3 normalize(vec3);
vec3 cross(vec3,vec3);

/* Light parent class */
class Light{
  public:
    int type;
    vec3 *v;
    RGB *intensity;
    int falloff;
    Light(float, float, float, float, float, float, int, int);
    void print();
};

/* Returned from a ray object intersection*/
class diffGeom{
  public:
    diffGeom(vec3, vec3, BRDF*, float);
    diffGeom();
    vec3 pos;
    vec3 normal;
    BRDF *brdf;
    float t;
    void print();
};

/* vector defined by the start and end locations */
class ray{
  public:
    ray(vec3, vec3, float, float);
    vec3 pos;
    vec3 dir;
    float t_min;
    float t_max;
    void print();
};

bool checkIntersection(ray*, float, float);

/* Parent class for scene geoemetry */
class sceneObject{
  public:
    virtual bool intersect(ray,diffGeom*,float) = 0;
};

/* Geometry Primitive */
class triangle: public sceneObject {
  public:
    vec3 *v1; vec3 *v2; vec3 *v3;
    vec3 *n1; vec3 *n2; vec3 *n3;
    BRDF *brdf;
    bool intersect(ray,diffGeom*,float);
    triangle(vec3*,vec3*,vec3*,BRDF*);
    triangle(vec3*,vec3*,vec3*,vec3*,vec3*,vec3*,BRDF*);
    void print();
};

class sphere: public sceneObject {
  public:
    sphere(float, float, float, float);
    sphere(vec3, float);
    bool intersect(ray,diffGeom*,float);
    vec3 center;
    float radius;
    BRDF *brdf;
};

#endif