//
// Created by ikegentz on 9/13/18.
//

#ifndef CS410_MODEL_H
#define CS410_MODEL_H

#include "wavefront_obj.h"
#include <glm/glm.hpp>
//////// REMOVE BEFORE SUBMITTING!!!!
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
//////// REMOVE BEFORE SUBMITTING!!!!
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
    glm::mat4 transformation_matrix;

    Model();
    std::string to_string();
    void load_from_str(std::string line);
    void build_transformation_matrix();
    void apply_transformation_matrix();
};

#endif //CS410_MODEL_H
