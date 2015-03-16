#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif
#ifndef CAMERA_H
#include "camera.hpp"
#endif
#ifndef SCENE_H
#include "scene.hpp"
#endif
#include "lodepng.h"
#include <cmath>
#include "input.hpp"
#ifndef COLOR_H
#include "color.hpp"
#endif

#ifndef CFLOAT_H
#include <cfloat>
#define CFLOAT_H
#endif
#define RECURSIVE_DEPTH 10
#define EPS 0.1


unsigned char* imageRGBA;
int pixelsWide = 1000;
int pixelsHigh = 500;
const char* outputFilename;
Scene s;

/* NOTE: Need to expand Light to -> directional, point, ambient subclasses */
RGB shading(diffGeom dg, Light* l, ray eyeRay){
  BRDF *brdf = dg.brdf;
  float r = 0; float g = 0; float b = 0;
  r += l->intensity->r * brdf->ka->r;
  g += l->intensity->g * brdf->ka->g;
  b += l->intensity->b * brdf->ka->b;
  vec3 lvec;
  /* Diffuse */
  if(l->type == POINT){
    lvec = (*l->v - dg.pos);
  } else if (l->type == DIR){
    lvec = -(*l->v);
  } else if(l->type == AMB){
    return RGB(r,g,b);
  }
  float dotln = (dg.normal)*(lvec); 
  float mdotln = fmax(dotln, 0);
  if (brdf->kd->r > 0) {
    r += (brdf->kd->r) * (l->intensity->r) * mdotln;
  }
  if (brdf->kd->g > 0) {
    g += (brdf->kd->g) * (l->intensity->g) * mdotln;
  }
  if (brdf->kd->b > 0) { 
    b += (brdf->kd->b) * (l->intensity->b) * mdotln;
  }

  vec3 reflection = -lvec;
  vec3 scalednorm = (dg.normal) * (2 * dotln);
  reflection = reflection + scalednorm;
  /* Normalize */
  float mag = sqrt(reflection.x * reflection.x + reflection.y * reflection.y + reflection.z * reflection.z);
  reflection = reflection * (1 / mag);

  /* Viewer ray */
  vec3 viewer = (dg.pos) - (eyeRay.pos);
  /* Specular */
  float dotvr = pow(fmax(viewer * reflection, 0), brdf->s);
  if (brdf->ks->r > 0) {
    r += (brdf->ks->r) * (l->intensity->r) * dotvr;
  }
  if (brdf->ks->g > 0) {
    g += (brdf->ks->g) * (l->intensity->g) * dotvr;
  }
  if (brdf->ks->b > 0) {
    b += (brdf->ks->b) * (l->intensity->b) * dotvr;
  }

  return RGB(r, g, b);
}

/* Function used to recursively trace rays */
RGB recursiveRT(ray r, int depth, RGB c){
  //r.print();
  if(depth != 0){
    diffGeom dg;
    if(s.trace(r, &dg)){
      /* Calculate Contribution from Light Sources */
      for(int i = 0; i < s.lights.size(); i++){
        Light* l = s.lights[i];
        ray shadowRay = ray(dg.pos, *(l->v) - dg.pos, EPS, dist(*l->v,dg.pos));
        if((l->type == AMB) || (!s.trace(shadowRay, NULL))){
          c += shading(dg, l, r);
          //c.print();
        }
      }
      /* Calculate Reflection Rays */
      if(dg.brdf->kr > 0){
        ray refl = ray(dg.pos, r.dir.reflect(dg.normal), EPS, FLT_MAX);
        RGB cr;
        c += *(dg.brdf->kr) * recursiveRT(refl, depth-1, cr);
      }
    }
  }
  return c;
}

void generateImage(){
  /* Iterate over all pixels */
  for(int j = 0; j < pixelsHigh; j++){
    for(int i = 0; i < pixelsWide; i++){
      /* Generate eye ray from pixel sample and initialize pixel color */
      ray eyeray = s.cam->getRay((i+0.5)/pixelsWide, (j+0.5)/pixelsHigh);
      RGB pixelColor = recursiveRT(eyeray, RECURSIVE_DEPTH, RGB(0,0,0));
      //std::cout << "(" << i << ", " << j << "): " << int(pixelColor.convert(RED)) << " - " << int(pixelColor.convert(GREEN)) << " - " << int(pixelColor.convert(BLUE)) << std::endl;
      imageRGBA[(pixelsWide * j + i)*4] = pixelColor.convert(RED);
      imageRGBA[(pixelsWide * j + i)*4 + 1] = pixelColor.convert(GREEN);
      imageRGBA[(pixelsWide * j + i)*4 + 2] = pixelColor.convert(BLUE);
      imageRGBA[(pixelsWide * j + i)*4 + 3] = 255;
    }
  }
}

int main(int argc, char** argv){
  parseInput(argc, argv, &s);
  imageRGBA = (unsigned char*) malloc(pixelsWide * pixelsHigh * 4 * sizeof(unsigned char));
  generateImage();
  std::vector<unsigned char> png;
  outputFilename = "output.png";
  if (!lodepng::encode(png, imageRGBA, pixelsWide, pixelsHigh)) {
    lodepng::save_file(png, outputFilename);
  } else {
    fprintf(stderr, "Failed to encode file.\n");
  }
}