#include "model.h"
#include "utils.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <math.h>

Model::Model() : transformation_matrix(glm::mat4(1.0f)), smooth(false) {}

std::string Model::to_string() const
{
    std::string output = "MODEL: " + this->wavefront_filename + "\n";
    output = output + "\taxis-angle (" + std::to_string(wx) + ", " + std::to_string(wy) + ", " + std::to_string(wz) + ")\n";
    output = output + "\ttheta: " + std::to_string(theta) + "\n";
    output = output + "\tscale: " + std::to_string(scale) + "\n";
    output = output + "\ttranslate (" + std::to_string(tx) + ", " + std::to_string(ty) + ", " + std::to_string(tz) + ")\n";

    return output;
}

void Model::load_from_str(std::string line)
{
    std::vector<std::string> tokens;
    Utils::tokenize_line(line, tokens);

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
    // should we smooth?
    this->smooth = tokens.at(9).compare("smooth") == 0;
    std::cout << "SMOOTH: " << smooth << std::endl;

    // extract wavefront filename
    this->wavefront_filename = tokens.at(tokens.size()-1);

    // load the actual 3D object, which extracts the material filename
    std::string mat_name = this->obj.load_wavefront_file(this->wavefront_filename.c_str());
    std::cout << "LOADED " << this->wavefront_filename << std::endl;
    //this->obj.print();

    this->material.load_material(mat_name);
    std::cout << "LOADED " << mat_name << std::endl;
    this->material.print();
}

void Model::create_little_m(const glm::vec3& normalized_w, glm::vec3& m) const
{
    unsigned smallest_index = 0;

    float sub0 = fabs(normalized_w[0]);
    float sub1 = fabs(normalized_w[1]);
    float sub2 = fabs(normalized_w[2]);

    // pick smallest term in w
    if(sub0 <= sub1 && sub0 <= sub2)
        smallest_index = 0;
    else if(sub1 <= sub0 && sub1 <= sub2)
        smallest_index = 1;
    else
        smallest_index = 2;

    // set smallest index to 1 and normalize to create m
    m = glm::vec3(normalized_w);
    m[smallest_index] = 1;
    glm::normalize(m);
}

void Model::build_rot_matrix(glm::mat4& rot) const
{
    glm::vec3 w(this->wx, this->wy, this->wz);
    w = glm::normalize(w);

    glm::vec3 m;
    this->create_little_m(w, m);

    glm::vec3 u = glm::cross(w, m);

    glm::vec3 v = glm::cross(w, u);

    glm::mat3 rot_mat_step1;
    rot_mat_step1[0] = glm::vec3(WavefrontObj::fix_neg_zero(u.x), WavefrontObj::fix_neg_zero(v.x), WavefrontObj::fix_neg_zero(w.x));
    rot_mat_step1[1] = glm::vec3(WavefrontObj::fix_neg_zero(u.y), WavefrontObj::fix_neg_zero(v.y), WavefrontObj::fix_neg_zero(w.y));
    rot_mat_step1[2] = glm::vec3(WavefrontObj::fix_neg_zero(u.z), WavefrontObj::fix_neg_zero(v.z), WavefrontObj::fix_neg_zero(w.z));


    glm::mat3 rot_mat_step1_tr = glm::transpose(rot_mat_step1);

    glm::mat3 rot_z;
    float radians = glm::radians(this->theta);
    rot_z[0] = glm::vec3(WavefrontObj::fix_neg_zero(cos(radians)), WavefrontObj::fix_neg_zero(sin(radians)), 0);
    rot_z[1] = glm::vec3(-WavefrontObj::fix_neg_zero(sin(radians)), WavefrontObj::fix_neg_zero(cos(radians)), 0);
    rot_z[2] = glm::vec3(0, 0, 1);

    glm::mat3 final_rot = rot_mat_step1_tr * rot_z * rot_mat_step1;

    rot[0] = glm::vec4(final_rot[0], 0);
    rot[1] = glm::vec4(final_rot[1], 0);
    rot[2] = glm::vec4(final_rot[2], 0);
    rot[3] = glm::vec4(0,0,0,1);
}

void Model::build_transformation_matrix()
{
    glm::vec3 translate_vector(this->tx, this->ty, this->tz);
    glm::mat4 translate_matrix = glm::translate(glm::mat4(1.0f), translate_vector);

    glm::vec3 scale_vector(this->scale, this->scale, this->scale);
    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale_vector);

    glm::vec3 w(this->wx, this->wy, this->wz);
    w = glm::normalize(w);

    //glm::mat4 rotate_matrix = glm::rotate(glm::radians(this->theta), w);
    glm::mat4 rotate_matrix;
    this->build_rot_matrix(rotate_matrix);

    this->transformation_matrix = translate_matrix * rotate_matrix * scale_matrix;
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
