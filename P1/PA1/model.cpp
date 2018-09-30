#include "model.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>

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

void Model::create_little_m(const glm::vec3& normalized_w, glm::vec3& m)
{
    // make sure we don't pick a 0 value
    const float ERROR = 0.00001f;
    unsigned smallest_index = 0;

    float sub0 = fabs(normalized_w[0]);
    float sub1 = fabs(normalized_w[1]);
    float sub2 = fabs(normalized_w[2]);

    // pick smallest term in w
    if(sub0 < sub1 && sub0 < sub2)
        smallest_index = 0;
    else if(sub1 < sub0 && sub1 < sub2)
        smallest_index = 1;
    else
        smallest_index = 2;

    // set smallest index to 1 and normalize to create m
    m = glm::vec3(normalized_w);
    m[smallest_index] = 1;
    glm::normalize(m);
}

void Model::build_transformation_matrix()
{
    glm::vec3 translate_vector(this->tx, this->ty, this->tz);
    glm::mat4 translate_matrix = glm::translate(glm::mat4(1.0f), translate_vector);

    glm::vec3 scale_vector(this->scale, this->scale, this->scale);
    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale_vector);

    glm::vec3 w(this->wx, this->wy, this->wz);
    w = glm::normalize(w);
    // std::cout << "NORMALIZED w: " << glm::to_string(w) << std::endl;
    //
    // glm::vec3 m;
    // this->create_little_m(w, m);
    // std::cout << "LITTLE m: " << glm::to_string(m) << std::endl;
    //
    // glm::vec3 u = glm::cross(w, m);
    // std::cout << "CROSS u: " << glm::to_string(u) << std::endl;
    //
    // glm::vec3 v = glm::cross(w, u);
    // std::cout << "CROSS v: " << glm::to_string(v) << std::endl;

    glm::mat4 rotate_matrix = glm::rotate(glm::radians(this->theta), w);
    std::cout << "THETA: " << this->theta << std::endl;
    std::cout << "ROTATE MATRIX: " << glm::to_string(rotate_matrix) << std::endl;

    this->transformation_matrix = translate_matrix * rotate_matrix * scale_matrix;

    std::cout << std::endl;
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
