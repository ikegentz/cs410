#include "wavefront_obj.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

void WavefrontObj::add_vertex(float x, float y, float z)
{
  Vertex vert;
  vert.x = x;
  vert.y = y;
  vert.z = z;
  vertices.push_back(vert);
}

void WavefrontObj::add_vertex_from_str(const std::vector<std::string> &tokens)
{
    std::cout << tokens.at(0) << std::endl;
}

void WavefrontObj::load_wavefront_file(const char* filename)
{
    std::ifstream infile(filename);
    std::string line;

    while (std::getline(infile, line))
    {
        std::string buffer;
        std::stringstream ss(line);

        std::vector<std::string> tokens;
        while(ss >> buffer)
            tokens.push_back(buffer);

        if(line.at(0) == '#')
            continue;
        else if(line.at(0) == 'v')
            this->add_vertex_from_str(tokens);
    }
}



Vertex::Vertex(): x(0), y(0), z(0) {}

Vertex::Vertex(float x_, float y_, float z_): x(x_), y(y_), z(z_) {}
