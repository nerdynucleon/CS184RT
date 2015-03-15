#ifndef COLOR_H
#include "color.hpp"
#endif

RGB::RGB(float rIn, float gIn, float bIn){
  r = rIn;
  g = gIn;
  b = bIn;
}

RGB RGB::operator*(RGB c1){
  return RGB(r*c1.r, c1.g*g, c1.b*b);
}

RGB::RGB(){
  r = 0;
  g = 0;
  b = 0;
}

bool RGB::operator>(float f){
  return ((r > f) && (b > f) && (g > f));
}

RGB RGB::operator+=(RGB c1){
  r += c1.r;
  b += c1.b;
  g += c1.g;
  return *this;
}

BRDF::BRDF(RGB* kaIn,  RGB* kdIn, RGB* ksIn, RGB* krIn, float sIn){
  ka = kaIn; kd = kdIn; ks = ksIn; kr = krIn; s = sIn;
}