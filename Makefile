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
debug: camera_g.o geometry_g.o color_g.o scene_g.o lodepng_g.o input_g.o
	g++ -g raytracer.cpp lodepng.o camera.o geometry.o color.o scene.o input.o -o main
camera_g.o: camera.cpp camera.hpp
	g++ -g -c camera.hpp camera.cpp
geometry_g.o: geometry.cpp geometry.hpp
	g++ -g -c geometry.hpp geometry.cpp
scene_g.o: scene.hpp scene.cpp
	g++ -g -c scene.hpp scene.cpp
objDecode_g.o: objDecode.cpp objDecode.hpp
	g++ -g -c objDecode.cpp objDecode.hpp
objTest_g.o: 
	g++ -g -Wall -I/usr/local/include -c objDecode.cpp
	g++ -g -L/usr/local/lib objDecode.o -lm -o objDecode
color_g.o: color.cpp color.hpp
	g++ -g -c color.hpp color.cpp
lodepng_g.o: lodepng.cpp lodepng.h 
	g++ -g -c lodepng.cpp lodepng.h
input_g.o: input.cpp input.hpp
	g++ -g -c input.hpp input.cpp
clean:
	rm *.o main *.gch output.png