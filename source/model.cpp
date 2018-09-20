#include "model.h"
#include <string>
#include <sstream>
#include <iostream>

Model::Model() : transformation_matrix(glm::mat4(1.0f)) {}

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
    // extract scale
    this->scale = std::stof(tokens.at(5));
    // extract translation
    this->tx = std::stof(tokens.at(6));
    this->ty = std::stof(tokens.at(7));
    this->tz = std::stof(tokens.at(8));
    // extract wavefront filename
    this->wavefront_filename = tokens.at(tokens.size()-1);
    // load the actual 3D object
    this->obj.load_wavefront_file(this->wavefront_filename.c_str());
}

void Model::build_transformation_matrix()
{
    glm::vec3 translate_vector(this->tx, this->ty, this->tz);
    std::cout << "TRANS VEC: " << glm::to_string(translate_vector) << std::endl;
    glm::mat4 translate_matrix = glm::translate(glm::mat4(1.0f), translate_vector);
    std::cout << "TRANS MAT: " << glm::to_string(translate_matrix) << std::endl;

    this->transformation_matrix = glm::mat4(translate_matrix);
    // std::cout << std::endl;
    // glm::vec3 w(this->wx, this->wy, this->wz);
    // std::cout << "ROTATE VEC: " << glm::to_string(w) << std::endl;
    // w = glm::normalize(w);
    // std::cout << "ROTATE Nrm: " << glm::to_string(w) << std::endl;
    //
    //
    //
    // glm::mat4 rotate_matrix = glm::rotate(glm::mat4(1.0f), this->theta, w);
    // std::cout << "ROTATE MAT: " << glm::to_string(rotate_matrix) << std::endl;
}

void Model::apply_transformation_matrix()
{
    for(unsigned i = 0; i < this->obj.vertices.size(); ++i)
    {
        Vertex vert = this->obj.vertices.at(i);
        glm::vec4 point = glm::vec4(vert.x, vert.y, vert.z, 1.0f);
        point = this->transformation_matrix * point;
        vert = Vertex(point.x, point.y, point.z);
        this->obj.vertices.at(i) = vert;
    }
}
