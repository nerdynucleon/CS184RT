README:
Jagdeep Manik: cs184-er
Alexandre Chong: cs184-bo

Platform: Mac OS X
Submission Location: cs184-er

http://www-inst.eecs.berkeley.edu/~cs184-er/ass2.html

All files located under output/image-xx.png
							   input-xx.png
							   notes-xx.png

REQUIRED (See notes):
========

Spheres/Ellipsoids:
image-01.png (Spheres)
image-09.png (Ellipsoid)

Polygons:
image-02.png (Triangles)
image-06.png (OBJ file)
  
Phong Shading Model:
image-03.png

Shadows:
image-04.png

Reflections:
image-05.png

Linear Transformations:
image-06.png (OBJ file)
image-01.png (Spheres)

Point and Directional Lights:
image-10.png (Point light)
image-11.png (Directional light)

Falloff:
image-02.png (No falloff)
image-07.png (Linear falloff)
image-12.png (Quadratic falloff)


EXTRA:
======

Antialiasing:
  Implemented using an average on a finer grid over the pixel. AA_LEVEL control the amount of subrays per pixel. The sub-rays are jittered randomly around their locations.

  For an example, see image_08 (AA_LEVEL=8) vs. image_05. image_06 was also rendered with AA=4.