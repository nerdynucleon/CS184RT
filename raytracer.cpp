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

#define RECURSIVE_DEPTH 10
#define EPS 0.1;


unsigned char imageRGBA*;
int pixelsWide;
int pixelsHigh;
char* outputFilename;
Scene s;

/* Parse commands line inputs */
void parseInput(int argc, char** argv){

}

int main(int argc, char** argv){
  parseInput(argc, argv);
  imageRGBA = (unsigned char *) malloc(sizeof(unsigned char) * 4 * pixelsHigh * pixelsWide);
  generateImage();
  std::vector<unsigned char> png;
  lodepng::encode(png, imageRGBA, pixelsWide, pixelsHigh);
  lodepng::save_file(png, outputFilename);
}

/* NOTE: Need to expand Light to -> directional, point, ambient subclasses */
RGB shading(diffGeom dg, Light* l, ray eyeRay){
  BRDF *brdf = dg.brdf;
  float r = 0; float g = 0; float b = 0;
  r += l->intensity.r * brdf->ka->r;
  g += l->intensity.g * brdf->ka->g;
  b += l->intensity.b * brdf->ka->b;

  /* Diffuse */
  vec3 lvec = (l->pos - dg.pos);
  float dotln = (dg.normal)*(lvec); 
  float mdotln = fmax(dotln, 0);
  if (brdf->kd->r > 0) {
    r += (brdf->kd->r) * (l->intensity.r) * mdotln;
  }
  if (brdf->kd->g > 0) {
    g += (brdf->kd->g) * (l->intensity.g) * mdotln;
  }
  if (brdf->kd->b > 0) { 
    b += (brdf->kd->b) * (l->intensity.b) * mdotln;
  }

  vec3 reflection = lvec * -1;
  vec3 scalednorm = (dg.normal) * (2 * dotln);
  reflection = reflection + scalednorm;
  /* Normalize */
  float mag = sqrt(reflection.x * reflection.x + reflection.y * reflection.y + reflection.z * reflection.z);
  reflection = reflection * (1 / mag);

  /* Viewer ray */
  vec3 viewer = (dg->pos) - (eyeRay->pos);
  /* Specular */
  float dotvr = pow(fmax(viewer * reflection, 0), brdf->s);
  if (brdf->ks->r > 0) {
    r += (brdf->ks->r) * (l->intensity.r) * dotvr;
  }
  if (brdf->ks->g > 0) {
    g += (brdf->ks->g) * (l->intensity.g) * dotvr;
  }
  if (brdf->ks->b > 0) {
    b += (brdf->ks->b) * (l->intensity.b) * dotvr;
  }

  return RGB(r, g, b);
}


/* Function used to recursively trace rays */
RGB recursiveRT(Ray r, int depth, color c){
  if(depth == 0){
    return;
  } else {
    diffGeom dg;
    if(s.trace(r, &dg)){
      /* Calculate Contribution from Light Sources */
      for(int i = 0; i < s.lights.size(); i++){
        Light* l = s.lights[i];
        ray shadowRay = ray(dg.pos,(l->pos - dg.pos), EPS, dist(l->pos,dg.pos));
        if(!s.trace(shadowRay, NULL)){
          c += shading(dg, l, r);
        }
      }
      /* Calculate Reflection Rays
      if(dg.kr > 0){

      }
      */
    }
  }
  return c;
}

void generateImage(){
  /* Iterate over all pixels */
  for(int j = 0; j < pixelsHigh; j++){
    for(int i = 0; i < pixelsWide; i++){
      /* Generate eye ray from pixel sample and initialize pixel color */
      Ray eyeray = s.cam.getRay((i+0.5)/pixelsWide, (j+0.5)/pixelsHigh);
      RGB pixelColor = recursiveRT(eyeray, RECURSIVE_DEPTH, RGB(0,0,0));
    }
  }
}