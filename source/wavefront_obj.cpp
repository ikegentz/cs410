#include "wavefront_obj.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

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
    this->add_vertex(std::stof(tokens.at(1)), std::stof(tokens.at(2)), std::stof(tokens.at(3)));
}

void WavefrontObj::add_face(int v1, int vn1, int v2, int vn2, int v3, int vn3)
{
  Face face(v1, vn1, v2, vn2, v3, vn3);
  faces.push_back(face);
}

void WavefrontObj::add_face_from_str(const std::vector<std::string> &tokens)
{
    std::pair<int,int> one = this->extract_face_values(tokens.at(1));
    int v1 = one.first;
    int vn1 = one.second;

    std::pair<int,int> two = this->extract_face_values(tokens.at(2));
    int v2 = two.first;
    int vn2 = two.second;

    std::pair<int,int> three = this->extract_face_values(tokens.at(3));
    int v3 = three.first;
    int vn3 = three.second;

    this->add_face(v1, vn1, v2, vn2, v3, vn3);
}

std::pair<int, int> WavefrontObj::extract_face_values(const std::string &token)
{
    std::string delimiter = "//";
    std::string cur = token;

    int v = std::stoi(cur.substr(0, cur.find(delimiter)));
    cur.erase(0, cur.find(delimiter) + delimiter.length());
    int vn = std::stoi(cur);

    return std::make_pair(v, vn);
}

void WavefrontObj::load_wavefront_file(const char* filename)
{
    std::ifstream infile(filename);
    if(infile.fail())
    {
        // try looking for file 1 level up
        std::string up_one = "../" + std::string(filename);
        infile = std::ifstream(up_one.c_str());
        if(infile.fail())
        {
            std::cerr << "Failed to open Wavefront file" << std::endl;
            return;
        }
    }
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
        else if(tokens.at(0) == "v")
            this->add_vertex_from_str(tokens);
        else if(tokens.at(0) == "f")
            this->add_face_from_str(tokens);
        else if(tokens.at(0) == "s")
            this->smoothing = line;
    }
}

void WavefrontObj::write_wavefront_file(const char* filename)
{
    std::ofstream outfile;
    outfile.open(filename);

    for(unsigned i = 0; i < this->vertices.size(); ++i)
        outfile << this->vertices.at(i).to_string() << "\n";

    outfile << this->smoothing << "\n";

    for(unsigned i = 0; i < this->faces.size(); ++i)
        outfile << this->faces.at(i).to_string() << "\n";

    outfile.close();
}


Vertex::Vertex(): x(0), y(0), z(0) {}
Vertex::Vertex(float x_, float y_, float z_): x(x_), y(y_), z(z_) {}

std::string Vertex::to_string()
{
    std::string ret = "v ";
    ret = ret + std::to_string(this->x) + " " + std::to_string(this->y) + " " + std::to_string(this->z);
    return ret;
}

Face::Face(int v1_, int vn1_, int v2_, int vn2_, int v3_, int vn3_): v1(v1_), vn1(vn1_), v2(v2_), vn2(vn2_), v3(v3_), vn3(vn3_) {}
Face::Face(const Face &face) : v1(face.v1), vn1(face.vn1), v2(face.v2), vn2(face.vn2), v3(face.v3), vn3(face.vn3) {}

std::string Face::to_string()
{
    std::string ret = "f ";
    ret = ret + std::to_string(this->v1) + "//" + std::to_string(this->vn1) + " ";
    ret = ret + std::to_string(this->v2) + "//" + std::to_string(this->vn2) + " ";
    ret = ret + std::to_string(this->v3) + "//" + std::to_string(this->vn3);

    return ret;
}
