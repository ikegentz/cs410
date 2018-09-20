#include <stdio.h>
#include "driver.h"

int main(int argc, char** argv)
{
    const char* driver_filename = argv[1];
    Driver driver(driver_filename);
}
