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

RGB RGB::operator*(float f) {
  return RGB(r*f, g*f, b*f);
}

RGB RGB::operator/(int f) {
  return RGB(r/f, g/f, b/f);
}

RGB RGB::operator/=(int f){
  return RGB(r/f, g/f, b/f);
}

RGB::RGB(){
  r = 0;
  g = 0;
  b = 0;
}

unsigned char RGB::convert(int color){
  float c;
  if(color == RED){
    c = r;
  } else if (color == GREEN){
    c = g;
  } else if (color == BLUE){
    c = b;
  }
  if(c <= 0){
    return 0;
  } else if (c >= 1){
    return 255;
  } else {
    return (unsigned char) (c * 255);
  }
}

void RGB::print() {
  std::cout << "RGB: {" << r << ", " << g << ", " << b << "}" << std::endl;
}

bool RGB::operator>(float f){
  return ((r > f) || (b > f) || (g > f));
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

void BRDF::print() {
  std::cout << "BRDF (" << this << "): " << std::endl;
  std::cout << "    sp: " << s << std::endl;
  std::cout << "    ka: "; ka->print();
  std::cout << "    kd: "; kd->print();
  std::cout << "    ks: "; ks->print();
  std::cout << "    kr: "; kr->print();
}