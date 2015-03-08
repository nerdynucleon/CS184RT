all: camera geometry color
	gcc raytracer.cpp -o main
camera: camera.cpp camera.hpp
	gcc -c camera.hpp camera.cpp
geometry: geometry.cpp geometry.hpp
	gcc -c geometry.hpp geometry.cpp
color: color.cpp color.hpp
	gcc -c color.hpp color.cpp
clean:
	rm *.o main

