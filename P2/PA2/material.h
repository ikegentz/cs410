//
// Created by ikegentz on 10/11/18.
//

#ifndef CS410_MATERIAL_H
#define CS410_MATERIAL_H

// Let us use some cool string casts
#define GLM_ENABLE_EXPERIMENTAL
// For un-named structs (make warnings go away since we know GLM works fine)
#define GLM_FORCE_SILENT_WARNINGS

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"

class Material
{
public:
    Material();
    glm::vec3 ka, kd;
    void load_material(std::string filename);
    void print();
};

#endif //CS410_MATERIAL_H
