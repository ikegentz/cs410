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
#include <string>
#include <iostream>

class Ray
{
private:
    glm::vec3 direction; // private so we can make sure it stays normalized

public:
    glm::vec3 position;

    // face vertices we of face we intersect with
    glm::vec3 Av;
    glm::vec3 Bv;
    glm::vec3 Cv;

    // last t value of that face we intersect with
    double last_t;
    // did we hit anything?
    bool hit;
    // did we hit a sphere?
    bool hit_sphere;

    Ray();
    void print() const;
    void set_direction(glm::vec3 direc);
    glm::vec3 get_direction() const { return glm::normalize(this->direction); }
    glm::vec3 get_direction_full() const {return this->direction;};
};

#endif //CS410_RAY_H
