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
    // the ray we shoot from the image plane into the scene
    Ray ray;
    glm::vec4 rgba;

    Pixel();
    void print() const;
};

#endif //CS410_PIXEL_H
