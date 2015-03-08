all: camera geometry
	gcc raytracer.cpp -o main
camera: camera.cpp camera.hpp
	gcc -c camera.hpp camera.cpp
geometry: geometry.cpp geometry.hpp
	gcc -c geometry.hpp geometry.cpp
clean:
	rm *.o main

