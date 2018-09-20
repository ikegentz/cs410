#include <stdio.h>
#include <iostream>
#include "driver.h"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cerr << "Please provide a driver file" << std::endl;
        exit(0);
    }
    const char* driver_filename = argv[1];
    Driver driver(driver_filename);
    driver.apply_model_transformations();
    driver.create_output_directory();
    driver.write_results();
}
