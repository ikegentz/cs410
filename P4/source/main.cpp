#include <stdio.h>
#include <iostream>
#include "driver.h"

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

    const char* driver_filename = argv[1];
    Driver driver(driver_filename);
    if(driver.read_driver_file() != 0)
    {
        std::cerr << "Failed while trying to parse driver file" << std::endl;
        exit(1);
    }
    driver.apply_model_transformations();
    driver.generate_image(argv[2]);
}
