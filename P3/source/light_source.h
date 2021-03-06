//
// Created by ikegentz on 10/10/18.
//

#ifndef CS410_LIGHT_SOURCE_H
#define CS410_LIGHT_SOURCE_H

// Let us use some cool string casts
#define GLM_ENABLE_EXPERIMENTAL
// For un-named structs (make warnings go away since we know GLM works fine)
#define GLM_FORCE_SILENT_WARNINGS

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"
#include <string>

class LightSource
{
public:
    LightSource();
    glm::vec3 position;
    glm::vec3 rgb_amount; // amount of R, G, B emitted by this light
    bool infinity;

    void load_light(std::string light);
    void print();
};

#endif //CS410_LIGHT_SOURCE_H
