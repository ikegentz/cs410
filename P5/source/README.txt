To compile the program, type 'make' in the same directory as the Makefile and the source code.
This will generate an executable called raytracer

Then run the program like ./raytracer driver0x.txt outputfile_name.ppm
passing in the name of the driver file and a name of the image file to generate.
This will then generate a ppm image file in the same directory as the executable.

driver00.txt is a simple scene. One sphere in the foreground, with a plane in the background.
You can observer here how the plane is refracted and distorted through the translucent sphere

driver01.txt is my "wow" project. I took the cage object from one of the earlier assignments and
wrapped it around a small solar system. You can see the 3 smaller 'planets' in the background, and a giant
'black hole' in the foreground. You'll notice how the 3 small 'planets' are refracted and distorted through
the 'black hole'. You can also see at the extreme edges of teh screen, the cage object. The wrapping of the
cage around the translucent spheres are what yield the cool 'texture' on the spheres, even though there
are no actual textures applied anywhere!
