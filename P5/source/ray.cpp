//
// Created by ikegentz on 10/11/18.
//

#include "ray.h"

Ray::Ray()
{
    this->direction = glm::vec3(0,0,1);
    this->position = glm::vec3(0,0,0);

    this->hit = false;
    this->last_t = -1;
    this->hit_sphere = false;

    this->Av = glm::vec3(0,0,0);
    this->Bv = glm::vec3(0,0,0);
    this->Cv = glm::vec3(0,0,0);
}

void Ray::print() const
{
    std::cout << "RAY\n" <<
    "\t" << "Position: " << glm::to_string(this->position) << "\n" <<
    "\t" << "Direction: " << glm::to_string(this->direction) << std::endl;
}

void Ray::set_direction(glm::vec3 direc)
{
    this->direction = direc;
}
