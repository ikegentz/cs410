//
// Created by ikegentz on 9/13/18.
//

#ifndef CS410_MODEL_H
#define CS410_MODEL_H

// Let us use some cool string casts
#define GLM_ENABLE_EXPERIMENTAL
// For un-named structs (make warnings go away since we know GLM works fine)
#define GLM_FORCE_SILENT_WARNINGS

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"

#include "wavefront_obj.h"
#include "material.h"
#include <string>

/*
    Stores model data and a Wavefront Object that we will apply transformation to
*/
class Model
{
public:
    float wx, wy, wz;
    float theta;
    float scale;
    float tx, ty, tz;
    std::string wavefront_filename;
    WavefrontObj obj;
    Material material;
    glm::mat4 transformation_matrix;
    bool smooth;

    Model();
    std::string to_string() const;
    void load_from_str(std::string line);
    void build_transformation_matrix();
    void apply_transformation_matrix();

private:
    void create_little_m(const glm::vec3& normalized_w, glm::vec3& m) const;
    void build_rot_matrix(glm::mat4& rot) const;
};

#endif //CS410_MODEL_H
