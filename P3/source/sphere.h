//
// Created by ikegentz on 10/30/18.
//

#ifndef SOURCE_SPHRE_H
#define SOURCE_SPHRE_H

// Let us use some cool string casts
#define GLM_ENABLE_EXPERIMENTAL
// For un-named structs (make warnings go away since we know GLM works fine)
#define GLM_FORCE_SILENT_WARNINGS

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"


class Sphere
{
public:
    void load_sphere(std::string line);
    void print();

    glm::vec3 position;
    float radius;
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    glm::vec3 Kr;
    static constexpr float PHONG = 16;
private:
};


#endif //SOURCE_SPHRE_H
