//
// Created by ikegentz on 10/11/18.
//

#ifndef CS410_RAY_H
#define CS410_RAY_H

// Let us use some cool string casts
#define GLM_ENABLE_EXPERIMENTAL
// For un-named structs (make warnings go away since we know GLM works fine)
#define GLM_FORCE_SILENT_WARNINGS

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"
#include "sphere.h"
#include <string>
#include <iostream>
#include <limits>

class Ray
{
private:
    glm::vec3 D; // private so we can make sure it stays normalized. direction

public:
    glm::vec3 L; // location

    // last t value of that face we intersect with
    double best_t;
    // did we hit anything?
    bool hit;
    // did we hit a sphere?
    bool hit_sphere;

    Sphere bestSphere; // TODO ROSS
    glm::vec3 bestPt; // TODO ROSS

    Ray();
    Ray(glm::vec3 L, glm::vec3 D);
    void print() const;
    void set_direction(glm::vec3 direc);
    glm::vec3 get_direction() const { return glm::normalize(this->D); }
    glm::vec3 get_direction_full() const {return this->D;};

    bool sphere_test(Sphere sph);// TODO ROSS
};

#endif //CS410_RAY_H
