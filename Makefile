all: camera.o geometry.o color.o scene.o lodepng.o input.o
	g++ raytracer.cpp lodepng.o camera.o geometry.o color.o scene.o input.o -o main
camera.o: camera.cpp camera.hpp
	g++ -c camera.hpp camera.cpp
geometry.o: geometry.cpp geometry.hpp
	g++ -c geometry.hpp geometry.cpp
scene.o: scene.hpp scene.cpp
	g++ -c scene.hpp scene.cpp
objDecode.o: objDecode.cpp objDecode.hpp
	g++ -c objDecode.cpp objDecode.hpp
objTest.o: 
	g++ -Wall -I/usr/local/include -c objDecode.cpp
	g++ -L/usr/local/lib objDecode.o -lm -o objDecode
color.o: color.cpp color.hpp
	g++ -c color.hpp color.cpp
lodepng.o: lodepng.cpp lodepng.h 
	g++ -c lodepng.cpp lodepng.h
input.o: input.cpp input.hpp
	g++ -c input.hpp input.cpp
clean:
	rm *.o main *.gch