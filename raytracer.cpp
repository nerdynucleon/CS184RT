#include <geometry.hpp>
#include <camera.hpp>


void parseInput(int argc, char** argv){

}

int main(int argc, char** argv){
  /* Initialize variables */
  parseInput(argc, argv);

  /* Iterate over all pixels */
  for(int j = 0; j < pixelsHigh; i++){
    for(int i = 0; i < pixelsWide; j++){
      Ray eyeray = cam.getRay((i+0.5)/pixelsWide, (j+0.5)/pixelsHigh);

    }
  }
}
