class triangle{};
class sphere{};
class diffGeom{};

class vec3{
  public:
    vec3(float x, float y, float z);
    float x; float y; float z;
};

class ray{
  vec3 v1;
  vec3 v2;
  public:
    ray(float, float, float, float, float, float);
    ray(vec3*, vec3*);
};

class sceneObject{
  public:
    diffGeom intersect(ray);
};

