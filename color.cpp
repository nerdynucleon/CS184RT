#ifndef COLOR_H
#include "color.hpp"
#endif

RGB::RGB(float rIn, float gIn, float bIn){
  r = rIn;
  g = gIn;
  b = bIn;
}

BRDF::BRDF(RGB* kaIn,  RGB* kdIn, RGB* ksIN, RGB* krIn, float sIn){
  ka = kaIn; kd = kdIn; ks = ksIn; kr = krIn; s = sIn;
}