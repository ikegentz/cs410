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
#include "utils.h"


Raytracer::Driver::Driver(const char *driver_filename) : recurse_depth(0)
{
    this->driver_filename = driver_filename;
}

int Raytracer::Driver::read_driver_file()
{
    std::ifstream infile(this->driver_filename);

    if(infile.fail())
    {
        std::cerr << "Failed to open driver file" << std::endl;
        return 1;
    }

    std::cout << "LOADING DRIVER FILE" << std::endl;

    std::string line;
    while (std::getline(infile, line))
    {
        if(line[0] == '#')
            continue;
        else if(line.find("eye") != std::string::npos)
        {
            camera.load_eye(line);
        }
        else if(line.find("look") != std::string::npos)
        {
            camera.load_look(line);
        }
        else if(line.find("bounds") != std::string::npos)
        {
            camera.load_bounds(line);
        }
        else if(line.find("recursionLevel") != std::string::npos)
        {
            std::vector<std::string> tokens;
            Utils::tokenize_line(line, tokens);
            this->recurse_depth = std::stoi(tokens[1]);
        }
        else if(line.find("ambient") != std::string::npos)
        {
            camera.load_ambient(line);
        }
        else if(line.find("light") != std::string::npos)
        {
            LightSource light;
            light.load_light(line);
            light_sources.push_back(light);
        }
        else if(line.find("sphere") != std::string::npos)
        {
            Sphere sphere;
            sphere.load_sphere(line);
            spheres.push_back(sphere);
        }
        else if(line.find("model") != std::string::npos)
        {
            Model model;
            // this also loads the wavefront file for the model transformation
            model.load_from_str(line);
            this->models.push_back(model);
        }
        else if(line.find("up") != std::string::npos)
        {
            camera.load_up(line);
        }
        else if(line.find("res") != std::string::npos)
        {
            camera.load_res(line);
        }
        else if(line.find("d ") != std::string::npos)
        {
            camera.load_d(line);
        }
    }

    image = Image(this->camera.bounds, this->camera.res);

    // Print out what we loaded for quick debugging
    camera.print();
    for(LightSource l : light_sources)
    { l.print(); }

    return 0;
}

void Raytracer::Driver::apply_model_transformations()
{
    std::cout << "APPLYING MODEL TRANSFORMATIONS" << std::endl;

    unsigned i;
    for(i = 0; i < this->models.size(); ++i)
    {
        this->models.at(i).build_transformation_matrix();
        this->models.at(i).apply_transformation_matrix();
        // mutliply verts by the matrix
    }

    std::cout << i << " MODELS TRANSFORMED" << std::endl;
    for(Model m : models)
    { std::cout << m.to_string() << std::endl; }
}

void Raytracer::Driver::generate_image(const char* filename)
{
    this->image.render_image(this->camera, this->models, this->spheres, this->light_sources);
    std::cout << "WRITING " << filename << std::endl;
    this->image.write_image(filename);
}

