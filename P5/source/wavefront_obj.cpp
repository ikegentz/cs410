#include "wavefront_obj.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <iomanip>      // std::setprecision
#include <cmath>


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

void WavefrontObj::add_face(int v1, int vt1, int vn1, int v2, int vt2, int vn2, int v3, int vt3, int vn3)
{
  Face face(v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3);
  faces.push_back(face);
}

void WavefrontObj::add_face_from_str(const std::vector<std::string> &tokens)
{
    std::tuple<int,int,int> one = this->extract_face_values(tokens.at(1));
    int v1 = std::get<0>(one);
    int vt1 = std::get<1>(one);
    int vn1 = std::get<2>(one);

    std::tuple<int,int,int> two = this->extract_face_values(tokens.at(2));
    int v2 = std::get<0>(two);
    int vt2 = std::get<1>(two);
    int vn2 = std::get<2>(two);

    std::tuple<int,int,int> three = this->extract_face_values(tokens.at(3));
    int v3 = std::get<0>(three);
    int vt3 = std::get<1>(three);
    int vn3 = std::get<2>(three);


    Face face(v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3);
    this->add_face(v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3);
//    std::cout << "ADDED FACE: \n" <<
//    "\t" <<face.to_string() << std::endl;
}

std::tuple<int, int, int> WavefrontObj::extract_face_values(const std::string &token)
{
    std::string delimiter = "//";
    std::string cur = token;
    int v, vn;
    int vt = -1;

    // There is no VT specified
    if(cur.find(delimiter) != std::string::npos)
    {
        v = std::stoi(cur.substr(0, cur.find(delimiter)));
        cur.erase(0, cur.find(delimiter) + delimiter.length());
        vn = std::stoi(cur);
    }
    // vt is specified
    else
    {
        delimiter = "/";
        v = std::stoi(cur.substr(0, cur.find(delimiter)));
        cur.erase(0, cur.find(delimiter) + delimiter.length());
        vt = std::stoi(cur.substr(0, cur.find(delimiter)));
        cur.erase(0, cur.find(delimiter) + delimiter.length());
        vn = std::stoi(cur);
    }

    return std::make_tuple(v, vt, vn);
}

std::string WavefrontObj::load_wavefront_file(const char* filename)
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
            exit(1);
        }
    }

    std::string mat_filename;

    std::string line;
    while (std::getline(infile, line))
    {
        if(line.length() == 0)
            continue;

        std::string buffer;
        std::stringstream ss(line);

        std::vector<std::string> tokens;
        while(ss >> buffer)
            tokens.push_back(buffer);

        if(line.at(0) == '#')
            continue;
        else if(line.find("mtllib") != std::string::npos)
            mat_filename = tokens.at(1);
        else if(line.find("vt") != std::string::npos)
            continue; //TODO actually do something with VT in P4
        else if(tokens.at(0) == "v")
            this->add_vertex_from_str(tokens);
        else if(tokens.at(0) == "f")
            this->add_face_from_str(tokens);
        else if(tokens.at(0) == "s")
            this->smoothing = line;

    }

    return mat_filename;
}

void WavefrontObj::write_wavefront_file(const char* filename)
{
    std::ofstream outfile;
    outfile.open(filename);

    for(unsigned i = 0; i < this->vertices.size(); ++i)
        outfile << std::setprecision(6) << this->vertices.at(i).to_string() << "\n";

    outfile << this->smoothing << "\n";

    for(unsigned i = 0; i < this->faces.size(); ++i)
        outfile << this->faces.at(i).to_string() << "\n";

    outfile.close();
}

 float WavefrontObj::fix_neg_zero(float in)
 {
     // fix -0.0000 type stuff
     if(fabs(in) < ERROR)
        return fabs(in);

    return in;
 }

 void WavefrontObj::print()
 {
    std::cout << "WAVEFRONT OBJ" << std::endl;
    for(Vertex v : vertices)
    {
        std::cout << "\t" << v.to_string() << std::endl;
    }
    for(Face f : faces)
    {
        std::cout << "\t" << f.to_string() << std::endl;
    }
 }


Vertex::Vertex(): x(0), y(0), z(0) {}
Vertex::Vertex(float x_, float y_, float z_): x(x_), y(y_), z(z_) {}

std::string Vertex::to_string()
{
    std::stringstream stream;
    stream << "v " << std::fixed << std::setprecision(7) <<
        WavefrontObj::fix_neg_zero(this->x) << " " <<
        WavefrontObj::fix_neg_zero(this->y) << " " <<
        WavefrontObj::fix_neg_zero(this->z);
    return stream.str();
}

Face::Face(int v1_, int vt1_, int vn1_, int v2_, int vt2_, int vn2_, int v3_, int vt3_, int vn3_): v1(v1_), vt1(vt1_), vn1(vn1_), v2(v2_), vt2(vt2_), vn2(vn2_), v3(v3_), vt3(vt3_), vn3(vn3_) {}
Face::Face(const Face &face) : v1(face.v1), vt1(face.vt1), vn1(face.vn1), v2(face.v2), vt2(face.vt2), vn2(face.vn2), v3(face.v3), vt3(face.vt3), vn3(face.vn3) {}

std::string Face::to_string()
{
    std::string ret = "f ";
    ret = ret + std::to_string(this->v1) + "/" + std::to_string(this->vt1) + "/" + std::to_string(this->vn1) + " ";
    ret = ret + std::to_string(this->v2) + "/" + std::to_string(this->vt2) + "/" + std::to_string(this->vn2) + " ";
    ret = ret + std::to_string(this->v3) + "/" + std::to_string(this->vt3) + "/" + std::to_string(this->vn3);

    return ret;
}
