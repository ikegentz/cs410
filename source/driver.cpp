//
// Created by ikegentz on 9/13/18.
//

#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>

#include "driver.h"

Driver::Driver(const char *driver_filename)
{
    std::ifstream infile(driver_filename);

    if(infile.fail())
    {
        std::cerr << "Failed to open driver file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(infile, line))
    {
        Model model;
        // this also loads the wavefront file for the model transformation
        model.load_from_str(line);
        this->models.push_back(model);
    }
}

void Driver::apply_model_transformations()
{
    for(unsigned i = 0; i < this->models.size(); ++i)
    {
        std::cout << models.at(i).to_string() << std::endl;
    }
}
