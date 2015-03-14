#ifndef COLOR_H
#define COLOR_H

class RGB{
  public:
    float r; float g; float b;
    RGB(float, float, float);
};

class BRDF{
  RGB *ka; RGB *kd; RGB *ks; RGB *kr;
  float s; /* exponent of specular term */ 
  public:
    BRDF(RGB*,RGB*,RGB*,RGB*,float);
};
#endif