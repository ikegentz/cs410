//
// Created by ikegentz on 10/30/18.
//
#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include "sphere.h"
#include "utils.h"

Sphere::Sphere()
{
    this->C = glm::vec3(0,0,0);
    this->r = 1;
    this->material = Material();
}

void Sphere::load_sphere(std::string line)
{
std::vector<std::string> tokens;
Utils::tokenize_line(line, tokens);

// extract C
this->C = glm::vec3(
        std::stof(tokens.at(1)),
        std::stof(tokens.at(2)),
        std::stof(tokens.at(3)));

// extract raduis
this->r = std::stof(tokens.at(4));

// extract C
this->material.ka = glm::vec3(
        std::stof(tokens.at(5)),
        std::stof(tokens.at(6)),
        std::stof(tokens.at(7)));

// extract C
this->material.kd = glm::vec3(
        std::stof(tokens.at(8)),
        std::stof(tokens.at(9)),
        std::stof(tokens.at(10)));

// extract C
this->material.ks = glm::vec3(
        std::stof(tokens.at(11)),
        std::stof(tokens.at(12)),
        std::stof(tokens.at(13)));

// extract C
this->material.kr = glm::vec3(
        std::stof(tokens.at(14)),
        std::stof(tokens.at(15)),
        std::stof(tokens.at(16)));

// extract ko
this->material.ko = glm::vec3(
        std::stof(tokens.at(17)),
        std::stof(tokens.at(18)),
        std::stof(tokens.at(19))
        );

this->material.eta = std::stof(tokens.at(20));

std::cout << "LOADED SPHERE" << std::endl;
this->print();
}

void Sphere::print()
{
    std::cout << "SPHERE: \n" <<
    "\tC: " << glm::to_string(this->C) << "\n" <<
    "\tr: " << this->r << "\n" <<
    "\tKa: " << glm::to_string(this->material.ka) << "\n" <<
    "\tKd: " << glm::to_string(this->material.kd) << "\n" <<
    "\tKs: " << glm::to_string(this->material.ks) << "\n" <<
    "\tKr: " << glm::to_string(this->material.kr) << "\n" <<
    "\tKo: " << glm::to_string(this->material.ko) << "\n" <<
    "\tPHONG: " << this->PHONG << std::endl;
}
glm::vec3 Sphere::refract_tray(glm::vec3 W, glm::vec3 pt, glm::vec3 N, float eta1, float eta2)
{
    float etar = eta1 / eta2;
    float a = -etar;
    float wn = glm::dot(W, N);
    float radsq = pow(etar, 2) * (pow(wn, 2) - 1) + 1;

    glm::vec3 T;
    if(radsq < 0.0f)
        T = glm::vec3(0.0f, 0.0f, 0.0f);
    else
    {
        float b = (etar * wn) - sqrt(radsq);
        T = a*W + b*N;
    }
    return T;
}

std::tuple<glm::vec3, glm::vec3> Sphere::refract_exit(glm::vec3 W, glm::vec3 pt, float eta_in, float eta_out)
{
    glm::vec3 T1 = refract_tray(W, pt, glm::normalize(pt - this->C), eta_out, eta_in);
    if(glm::length(T1) < 0.1f)
    {
        glm::vec3 pos(0.0f, 0.0f, 0.0f);
        glm::vec3 direc(0.0f, 0.0f, 0.0f);
        return std::make_tuple(pos, direc);
    }
    else
    {
        glm::vec3 exit = pt + 2 * glm::dot( (this->C - pt), T1 ) * T1;
        glm::vec3 Nin = glm::normalize(this->C - exit);
        glm::vec3 T2 = refract_tray(-T1, exit, Nin, eta_in, eta_out);
        return std::make_tuple(exit, T2); // T2 will be all zeros if it isn't successful
    }
}
