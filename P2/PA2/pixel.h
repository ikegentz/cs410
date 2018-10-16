//
// Created by ikegentz on 10/11/18.
//

#ifndef CS410_PIXEL_H
#define CS410_PIXEL_H

#include "ray.h"

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"
#include <string>
#include <iostream>

class Pixel
{
public:
    // last t value of that face we intersect with
    double last_t;
    // did we hit anything?
    bool hit;
    // the ray we shoot from the image plane into the scene
    Ray ray;
    glm::vec4 rgba;

    // face vertices we of face we interssect with
    glm::vec3 Av;
    glm::vec3 Bv;
    glm::vec3 Cv;

    Pixel();
    void print() const;
};

#endif //CS410_PIXEL_H
