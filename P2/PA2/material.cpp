//
// Created by ikegentz on 10/11/18.
//

#include "material.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <fstream>
#include <iostream>

void Material::load_material(std::string filename)
{
    std::ifstream infile(filename);
    if(infile.fail())
    {
        // try looking for file 1 level up
        std::string up_one = "../" + std::string(filename);
                infile = std::ifstream(up_one.c_str());
        if(infile.fail())
        {
            std::cerr << "Failed to open materials file" << std::endl;
            exit(1);
        }
    }

    std::string line;
    while (std::getline(infile, line))
    {
        if(line.size() == 0)
            continue;

        std::vector <std::string> tokens;
        utils::tokenize_line(line, tokens);

        if(line[0] == '#')
            continue;
        else if(tokens.at(0).find("Ka") != std::string::npos)
        {
            ka = glm::vec3(
                    std::stof(tokens.at(1)),
                    std::stof(tokens.at(2)),
                    std::stof(tokens.at(3))
                    );
        }
        else if(tokens.at(0).find("Kd") != std::string::npos)
        {
            kd = glm::vec3(
                    std::stof(tokens.at(1)),
                    std::stof(tokens.at(2)),
                    std::stof(tokens.at(3))
            );
        }
        else
            continue;
    }
}

void Material::print()
{
    std::cout << "MATERIAL" << "\n" <<
    "\t" << "Ka: " << glm::to_string(this->ka) << "\n" <<
    "\t" << "Kd: " << glm::to_string(this->kd) << std::endl;
}