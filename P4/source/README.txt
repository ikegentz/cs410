To compile the program, type 'make' in the same directory as the Makefile and the source code.
This will generate an executable called raytracer

Then run the program like ./raytracer driver0x.txt outputfile_name.ppm
passing in the name of the driver file and a name of the image file to generate.
This will then generate a ppm image file in the same directory as the executable.

driver03.txt has a floor, wall, 2 spheres, and a cone. You can see the shadows from the spheres on the floor, wall, and cone object.

My reflection isn't quite right, but you can see the reflection of the spheres on the floor, as well as the blue sphere reflecting on the yellow sphere

Smoothing was not working so I didn't include it in this version of the assignment