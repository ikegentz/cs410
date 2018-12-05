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
    this->position = glm::vec3(0,0,0);
    this->radius = 1;
    this->material = Material();
}

void Sphere::load_sphere(std::string line)
{
std::vector<std::string> tokens;
Utils::tokenize_line(line, tokens);

// extract position
this->position = glm::vec3(
        std::stof(tokens.at(1)),
        std::stof(tokens.at(2)),
        std::stof(tokens.at(3)));

// extract raduis
this->radius = std::stof(tokens.at(4));

// extract position
this->material.ka = glm::vec3(
        std::stof(tokens.at(5)),
        std::stof(tokens.at(6)),
        std::stof(tokens.at(7)));

// extract position
this->material.kd = glm::vec3(
        std::stof(tokens.at(8)),
        std::stof(tokens.at(9)),
        std::stof(tokens.at(10)));

// extract position
this->material.ks = glm::vec3(
        std::stof(tokens.at(11)),
        std::stof(tokens.at(12)),
        std::stof(tokens.at(13)));

// extract position
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
    "\tPosition: " << glm::to_string(this->position) << "\n" <<
    "\tRadius: " << this->radius << "\n" <<
    "\tKa: " << glm::to_string(this->material.ka) << "\n" <<
    "\tKd: " << glm::to_string(this->material.kd) << "\n" <<
    "\tKs: " << glm::to_string(this->material.ks) << "\n" <<
    "\tKr: " << glm::to_string(this->material.kr) << "\n" <<
    "\tPHONG: " << this->PHONG << std::endl;
}