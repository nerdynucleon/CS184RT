#include <geometry.hpp>
#include <camera.hpp>


void parseInput(int argc, char** argv){

}

Scene s;
float epsilon;

int main(int argc, char** argv){
  /* Initialize variables */
  parseInput(argc, argv);

  /* Iterate over all pixels */
  for(int j = 0; j < pixelsHigh; i++){
    for(int i = 0; i < pixelsWide; j++){
      Ray eyeray = s.cam.getRay((i+0.5)/pixelsWide, (j+0.5)/pixelsHigh);
      diffGeom dg; s.trace(eyeray, &dg);
      if(dg != NULL){

      } else {

      }
    }
  }
}
