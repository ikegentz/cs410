//
// Created by ikegentz on 9/13/18.
//

#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <iomanip>

#include "driver.h"

Driver::Driver(const char *driver_filename)
{
    this->driver_filename = driver_filename;
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

        this->models.at(i).build_transformation_matrix();
        this->models.at(i).apply_transformation_matrix();
        // mutliply verts by the matrix
    }
}

void Driver::create_output_directory()
{
    std::string dir_name = std::string(this->driver_filename);

    unsigned start_index = dir_name.find("d");
    dir_name = dir_name.substr(start_index);
    dir_name = dir_name.substr(0, dir_name.find("."));

    this->output_dir = dir_name;
    std::string temp = std::string("mkdir -p " + dir_name).c_str();

    system(temp.c_str());
}

void Driver::write_results()
{
    for(unsigned i = 0; i < this->models.size(); ++i)
    {
        // build the base name
        Model m = this->models.at(i);
        std::string filename = this->output_dir + "/" + m.wavefront_filename;
        filename = filename.substr(0,filename.find(".")) + "_mw";
        // append a number in case of multiple transformations to same .obj
        unsigned model_num = 0;
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << std::to_string(model_num);
        std::string final_filename = filename + ss.str() + ".obj";
        // keep checking if we've already written that file, and make new name if so
        std::ifstream infile(final_filename);
        while(infile.good())
        {
            ++model_num;
            ss = std::stringstream();
            ss << std::setw(2) << std::setfill('0') << std::to_string(model_num);
            final_filename = filename + ss.str() + ".obj";
            infile = std::ifstream(final_filename);
        }

        m.obj.write_wavefront_file(final_filename.c_str());
    }
}
