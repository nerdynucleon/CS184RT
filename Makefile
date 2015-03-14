all: camera geometry color scene png
	g++ raytracer.cpp lodepng.o camera.o geometry.o color.o scene.o -o main
camera: camera.cpp camera.hpp
	g++ -c camera.hpp camera.cpp
geometry: geometry.cpp geometry.hpp
	g++ -c geometry.hpp geometry.cpp
scene: scene.hpp scene.cpp
	g++ -c scene.hpp scene.cpp
objDecode: objDecode.cpp objDecode.hpp
	g++ -c objDecode.cpp objDecode.hpp
objTest: 
	g++ -Wall -I/usr/local/include -c objDecode.cpp
	g++ -L/usr/local/lib objDecode.o -lm -o objDecode
color: color.cpp color.hpp
	g++ -c color.hpp color.cpp
png: lodepng-master/lodepng_util.cpp lodepng-master/lodepng_util.h lodepng-master/lodepng.cpp lodepng-master/lodepng.h lodepng-master/pngdetail.cpp
	g++ -c lodepng-master/pngdetail.cpp
	g++ -c lodepng-master/lodepng.cpp lodepng-master/lodepng.h
	g++ -c lodepng-master/lodepng_util.cpp lodepng-master/lodepng_util.h
clean:
	rm *.o main *.gch