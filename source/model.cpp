#include "model.h"
#include <string>
#include <sstream>


std::string Model::to_string()
{
    std::string output = "Model: " + this->wavefront_filename + "\n";
    output = output + "\taxis-angle (" + std::to_string(wx) + ", " + std::to_string(wy) + ", " + std::to_string(wz) + ")\n";
    output = output + "\ttheta: " + std::to_string(theta) + "\n";
    output = output + "\tscale: " + std::to_string(scale) + "\n";
    output = output + "\ttranslate (" + std::to_string(tx) + ", " + std::to_string(ty) + ", " + std::to_string(tz) + ")\n";

    return output;
}

void Model::load_from_str(std::string line)
{
    std::string buffer;
    std::stringstream ss(line);

    // tokenize the line to grab model data
    std::vector<std::string> tokens;
    while(ss >> buffer)
        tokens.push_back(buffer);

    // extract axis-angles
    this->wx = std::stof(tokens.at(1));
    this->wy = std::stof(tokens.at(2));
    this->wz = std::stof(tokens.at(3));
    // extract rotation angle amount
    this->theta = std::stof(tokens.at(4));
    //extract wavefront filename
    this->wavefront_filename = tokens.at(tokens.size()-1);
    //load the actual 3D object
    this->obj.load_wavefront_file(this->wavefront_filename.c_str());
}
