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
#ifndef INPUT_H
#include "input.hpp"
#endif
#ifndef COLOR_H
#include "color.hpp"
#endif

#ifndef CFLOAT_H
#include <cfloat>
#define CFLOAT_H
#endif
#define RECURSIVE_DEPTH 10
#define EPS 0.1
#define AA false
#define AA_LEVEL 1

unsigned char* imageRGBA;
int pixelsWide = 1000;
int pixelsHigh = 1000;
const char* outputFilename;
Scene s;

RGB shading(diffGeom dg, Light* l, ray eyeRay){
  BRDF *brdf = dg.brdf;
  vec3 normal = normalize(dg.normal);
  vec3 lvec;
  RGB rgb = RGB(0, 0, 0);

  /* Diffuse */
  if(l->type == POINT){
    lvec = normalize(*l->v - dg.pos);
  } else if (l->type == DIR){
    lvec = normalize(-(*l->v));
  } else if(l->type == AMB){
    return (*brdf->ka) * (*l->intensity); 
  }
  float dotln = normal*(lvec); float falloff;
  float mdotln = fmax(dotln, 0);
  if (brdf->kd > 0) {
    rgb += (*brdf->kd) * (*l->intensity) * mdotln;
  }

  /* Calculate falloff */
  if (l->falloff == FALLOFF_NONE) { falloff = 1; }
  else if (l->falloff == FALLOFF_LINEAR) { falloff = 1/(dist(dg.pos, *(l->v))); }
  else { falloff = 1/pow(dist(dg.pos, *(l->v)), 2); }

  /* Specular component */
  if (brdf->ks > 0) {
    vec3 reflection = normalize(-lvec + normal * (2 * dotln));
    vec3 viewer = normalize(eyeRay.pos - dg.pos);
    float dotvr = pow(fmax(viewer * reflection, 0), brdf->s);
    rgb += (*brdf->ks) * (*l->intensity) * dotvr; 
  }

  return rgb*falloff;
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
        vec3 lvec; float distance;
        if (l->type == POINT) {
          lvec = normalize(*(l->v) - dg.pos); distance = dist(*l->v, dg.pos);
        } else {
          lvec = normalize(-(*l->v)); distance = FLT_MAX;
        }
        ray shadowRay = ray(dg.pos, lvec, EPS, distance);
        if((l->type == AMB) || (!s.trace(shadowRay, NULL))){
          c += shading(dg, l, r);
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
  #pragma omp parallel for
  for(int j = 0; j < pixelsHigh; j++){
    for(int i = 0; i < pixelsWide; i++){
      /* Generate eye ray from pixel sample and initialize pixel color */
      RGB pixelColor =  RGB(0,0,0);
      if(AA){
        for(int m = 0; m < AA_LEVEL; m++){
          for(int n = 0; n < AA_LEVEL; n++){
            float rand1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            rand1 = (2*m + 1)/(AA_LEVEL*2) + rand1/AA_LEVEL;
            float rand2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            rand1 = (2*n + 1)/(AA_LEVEL*2) + rand1/AA_LEVEL;
            //printf("%f %f\n",rand1-.5, rand2-.5 );
            ray eyeray = s.cam->getRay((i+rand1)/pixelsWide, (j+rand2)/pixelsHigh);
            pixelColor += recursiveRT(eyeray, RECURSIVE_DEPTH, RGB(0,0,0))/(AA_LEVEL*AA_LEVEL);
          }       
        }
      } else {
        ray eyeray = s.cam->getRay((i+0.5)/pixelsWide, (j+0.5)/pixelsHigh);
        pixelColor += recursiveRT(eyeray, RECURSIVE_DEPTH, RGB(0,0,0));
      }
      imageRGBA[(pixelsWide * (pixelsHigh - 1 - j) + i)*4] = pixelColor.convert(RED);
      imageRGBA[(pixelsWide * (pixelsHigh - 1 - j) + i)*4 + 1] = pixelColor.convert(GREEN);
      imageRGBA[(pixelsWide * (pixelsHigh - 1 - j) + i)*4 + 2] = pixelColor.convert(BLUE);
      imageRGBA[(pixelsWide * (pixelsHigh - 1 - j) + i)*4 + 3] = 255;
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