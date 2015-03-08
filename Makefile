all: camera objDecode raytracer geometry
camera:
geometry: geometry.cpp geometry.hpp
	gcc geometry.hpp geometry.cpp 

