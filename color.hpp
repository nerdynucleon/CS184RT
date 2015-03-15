#ifndef COLOR_H
#define COLOR_H

class RGB{
  public:
    float r; float g; float b;
    RGB operator+=(RGB);
    bool operator>(float);
    RGB operator*(RGB);
    RGB(float, float, float);
    RGB();
};

class BRDF{ 
  public:
  	RGB *ka; RGB *kd; RGB *ks; RGB *kr;
  	float s; /* exponent of specular term */
    BRDF(RGB*,RGB*,RGB*,RGB*,float);
};
#endif