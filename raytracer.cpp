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

/* Function used to recursively trace rays */
color recursiveRT(ray r, int depth, color c){
  if(depth == 0){
    return;
  } else {
    diffGeom dg;
    if(scene.trace(r, &dg)){
      /* Calculate Contribution from Light Sources */
      for(int i = 0; i < s.lights.size(); i++){
        Light* l = s.lights[i];
        ray shadowRay = ray(dg.pos,(l->pos - dg.pos), EPS, dist(l->pos,dg.pos));
        if(!s.trace(shadowRay, NULL)){
          c += shading(dg);
        }
      }
      /* Calculate Reflection Rays
      if(dg.kr > 0){

      }
      */
    }
  }
}

void generateImage(){
  /* Iterate over all pixels */
  for(int j = 0; j < pixelsHigh; j++){
    for(int i = 0; i < pixelsWide; i++){
      /* Generate eye ray from pixel sample and initialize pixel color */
      ray eyeray = s.cam.getRay((i+0.5)/pixelsWide, (j+0.5)/pixelsHigh);
      RGB pixelColor = recursiveRT(eyeray, RECURSIVE_DEPTH, pixelColor);
    }
  }
}