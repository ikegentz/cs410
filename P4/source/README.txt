To compile the program, type 'make' in the same directory as the Makefile and the source code.
This will generate an executable called raytracer

Then run the program like ./raytracer driver0x.txt outputfile_name.ppm
passing in the name of the driver file and a name of the image file to generate.
This will then generate a ppm image file in the same directory as the executable.

driver03.txt is based off of driver01.txt. It adds an extra gigantic sphere in the background to show sphere occulsion.

driver04.txt is based off of driver02.txt. It adds an extra cage and moves the ball around to show the ball's shadow on both cages. I also moved the blue light down a lot lower in the image.
Since there are no faces on the cages that both hit the blue light and arent occluded by other faces, we dont see the blue light on the cage. We can however tell the blue light's placement
by looking at the diffuse and specular reflection on the ball.

Final note, I didn't quite get shadows from polygon faces working right, so I left it out and only included shadows from spheres.
