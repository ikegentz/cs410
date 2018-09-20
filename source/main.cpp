#include <stdio.h>
#include "driver.h"

int main(int argc, char** argv)
{
    const char* driver_filename = argv[1];
    Driver driver(driver_filename);
    driver.apply_model_transformations();
    driver.create_output_directory();
    driver.write_results();
}
