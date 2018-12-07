//
// Created by ikegentz on 10/11/18.
//

#include "ray.h"

Ray::Ray()
{
    this->D = glm::vec3(0,0,1);
    this->L = glm::vec3(0,0,0);

    this->best_t = std::numeric_limits<float>::max();

    this->bestPt = glm::vec3(0,0,0);
    this->bestSphere = nullptr;
}

Ray::Ray(glm::vec3 L, glm::vec3 D)
{
    this->L = L;
    this->D = glm::normalize(D);
    this->best_t = std::numeric_limits<float>::max();
    this->bestPt = glm::vec3(0,0,0);
    this->bestSphere = nullptr;
}

void Ray::print() const
{
//    std::cout << "RAY\n" <<
//    "\t" << "Position: " << glm::to_string(this->position) << "\n" <<
//    "\t" << "Direction: " << glm::to_string(this->direction) << std::endl;
}

void Ray::set_direction(glm::vec3 direc)
{
    this->D = direc;
}

bool Ray::sphere_test(Sphere& sph)
{
    glm::vec3 Tv = sph.C - this->L;
    float v = glm::dot(Tv, this->D);
    float csq = glm::dot(Tv, Tv);
    float disc = sph.r*sph.r - (csq - v*v);
    if(disc > 0)
    {
        float tval = v - sqrt(disc);
        if(tval < this->best_t && tval > 0.001f)
        {
            this->best_t = tval;
            this->bestSphere = &sph;
            this->bestPt = this->L + tval*this->D;
            return true;
        }
    }
    return false;
}// TODO ROSS
