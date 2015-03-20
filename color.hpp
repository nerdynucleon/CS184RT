#ifndef COLOR_H
#define COLOR_H
#include <iostream>

#define RED 0
#define GREEN 1
#define BLUE 2

class RGB{
  public:
    float r; float g; float b;
    RGB operator+=(RGB);
    bool operator>(float);
    RGB operator*(RGB);
    RGB operator*(float);
    RGB(float, float, float);
    RGB();
    void print();
    unsigned char convert(int);
};

class BRDF{ 
  public:
  	RGB *ka; RGB *kd; RGB *ks; RGB *kr;
  	float s; /* exponent of specular term */
    BRDF(RGB*,RGB*,RGB*,RGB*,float);
    void print();
};
#endif