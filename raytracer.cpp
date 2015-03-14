#ifndef GEOMETRY_H
#include "geometry.hpp"
#endif
#ifndef CAMERA_H
#include "camera.hpp"
#endif
#ifndef SCENE_H
#include "scene.hpp"
#endif

#define RECURSIVE_DEPTH 10
#define EPS 0.1;

Scene s;

/* Parse commands line inputs */
void parseInput(int argc, char** argv){

}



int main(int argc, char** argv){
  parseInput(argc, argv);
  generateImage();
}

/* Function used to recursively trace rays */
color recursiveRT(Ray r, int depth, color c){
  diffGeom dg; s.trace(eyeray, &dg);
}

void generateImage(){
  /* Iterate over all pixels */
  for(int j = 0; j < pixelsHigh; i++){
    for(int i = 0; i < pixelsWide; j++){
      /* Generate eye ray from pixel sample and initialize pixel color */
      Ray eyeray = s.cam.getRay((i+0.5)/pixelsWide, (j+0.5)/pixelsHigh);
      RGB pixelColor = RGB(0,0,0);
      recursiveRT(eyeray, RECURSIVE_DEPTH, pixelColor);
      
      if(dg != NULL){

      } else {

      }
    }
  }
}