#include "color.hpp"

RGB::RGB(float rIn, float gIn, float bIn){
  r = rIn;
  g = gIn;
  b = bIn;
}

Light::Light(float intensityIn, float rIn, float gIn, float bIn):
  color(rIn, gIn, bIn){
    intensity = intensityIn;

}

Light::Light(float intensityIn, RGB *colorIn):
  color(colorIn->r, colorIn->g, colorIn->b){
    intensity = intensityIn;
}

