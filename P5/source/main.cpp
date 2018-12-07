#include <stdio.h>
#include <iostream>
#include "driver.h"
#include "raytracer_data.h"

int main(int argc, char** argv)
{
    if(argc == 1)
    {
        std::cerr << "Please provide a driver file" << std::endl;
        exit(0);
    }
    else if(argc == 2)
    {
        std::cerr << "Please provide a name for the ouput .ppm file" << std::endl;
        exit(0);
    }

    // class to hold all of our 'global' data
    RaytracerData data;

    // create driver to read in program instructions
    const char* driver_filename = argv[1];
    Driver driver(driver_filename, &data);
    if(driver.read_driver_file() != 0)
    {
        std::cerr << "Failed while trying to parse driver file" << std::endl;
        exit(1);
    }

    // position models and spheres in the scene
    driver.apply_model_transformations();

    Image image(&data);//data.camera.bounds, data.camera.res);

    image.render_image();
    std::cout << "WRITING " << argv[2] << std::endl;
    image.write_image(argv[2]);
}
