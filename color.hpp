#ifndef COLOR_H
#define COLOR_H

/* forward declarations of classes */

class RGB{
  public:
    float r; float g; float b;
    RGB(float, float, float);
};

class BRDF{
  RGB *ka; RGB *kd; RGB *ks;
  float s; /* exponent of specular term */ 
  public:
    BRDF(RGB*,RGB*,RGB*,float);
};
#endif