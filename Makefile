all: camera geometry
	gcc raytracer.cpp -o main
camera: camera.cpp camera.hpp
	gcc -c camera.hpp camera.cpp
geometry: geometry.cpp geometry.hpp
	gcc -c geometry.hpp geometry.cpp
objDecode: objDecode.cpp objDecode.hpp
	gcc -c objDecode.cpp objDecode.hpp
objTest:
	g++ -Wall -I/usr/local/include -c objDecode.cpp
	g++ -L/usr/local/lib objDecode.o -lm -o objDecode
clean:
	rm *.o main

