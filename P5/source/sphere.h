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
#include "material.h"
#include <tuple>

class Sphere
{
public:
    Sphere();
    void load_sphere(std::string line);
    void print();

    glm::vec3 C;
    float r;
    Material material;
    static constexpr float PHONG = 16;
    static constexpr float AIR_ETA = 1.0f;

    // returns direction of the ray
    glm::vec3 refract_tray(glm::vec3 W, glm::vec3 pt, glm::vec3 N, float eta1, float eta2);
    // returns point of a ray and the ray's direction
    std::tuple<glm::vec3, glm::vec3> refract_exit(glm::vec3 W, glm::vec3 pt, float eta_in, float eta_out);

private:
};


#endif //SOURCE_SPHRE_H
