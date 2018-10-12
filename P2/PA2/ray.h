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
    Ray();
    void print() const;
    void set_direction(const glm::vec3 &direc);
    glm::vec3 get_direction() const { return this->direction; }
};

#endif //CS410_RAY_H
