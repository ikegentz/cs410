//
// Created by ikegentz on 9/13/18.
//

#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sstream>

#include "driver.h"

Driver::Driver(char *driver_filename)
{
    std::ifstream infile(driver_filename);
    std::string line;

    while (std::getline(infile, line))
    {
        this->add_model_from_str(line);

      //
      // if (line.at(0) == '#')
      //     continue;
      // else if(line.at(0) == 'v')
      // {
      //   for(int i = 0; i < tokens.size(); ++i)
      //   {
      //     //extract and push into vertices vector
      //     std::cout << tokens.at(i) << std::endl;
      //   }
      // }



      // for(int i = 0; i < tokens.size(); ++i)
      // {
      //   //  std::cout << tokens.at(i) << std::endl;
      // }
    }
}

void Driver::add_model_from_str(std::string line)
{
    std::string buffer;
    std::stringstream ss(line);

    // tokenize the line to grab model data
    std::vector<std::string> tokens;
    while(ss >> buffer)
        tokens.push_back(buffer);

    // the new model
    Model model;

    // extract axis-angles
    model.wx = std::stof(tokens.at(1));
    model.wy = std::stof(tokens.at(2));
    model.wz = std::stof(tokens.at(3));
    // extract rotation angle amount
    model.theta = std::stof(tokens.at(4));
    //extract wavefront filename
    model.wavefront_filename = tokens.at(tokens.size()-1);
    std::cout << model.to_string() << std::endl;
}
