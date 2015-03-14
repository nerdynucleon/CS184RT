all: camera geometry color
	gcc raytracer.cpp -o main
camera: camera.cpp camera.hpp
	gcc -c camera.hpp camera.cpp
geometry: geometry.cpp geometry.hpp
	gcc -c geometry.hpp geometry.cpp
scene: scene.hpp scene.cpp
	gcc -c scene.hpp scene.cpp
objDecode: objDecode.cpp objDecode.hpp
	gcc -c objDecode.cpp objDecode.hpp
objTest:
	g++ -Wall -I/usr/local/include -c objDecode.cpp
	g++ -L/usr/local/lib objDecode.o -lm -o objDecode
color: color.cpp color.hpp
	gcc -c color.hpp color.cpp
clean:
	rm *.o main

