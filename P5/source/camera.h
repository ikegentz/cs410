//
// Created by ikegentz on 10/10/18.
//

#ifndef CS410_CAMERA_H
#define CS410_CAMERA_H

// Let us use some cool string casts
#define GLM_ENABLE_EXPERIMENTAL
// For un-named structs (make warnings go away since we know GLM works fine)
#define GLM_FORCE_SILENT_WARNINGS

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"
#include <string>

class Camera
{
public:
    glm::vec3 eye;
    void load_eye(std::string eye);

    glm::vec3 look;
    void load_look(std::string look);

    glm::vec3 up;
    void load_up(std::string up);

    double d; // focal length -- distance from focal point (camera/eye) to image plane
    void load_d(std::string d);

    glm::vec4 bounds; // bounds on image rectangle (world coordinates)
    void load_bounds(std::string bounds);

    glm::vec2 res; // pixel resolution (e.g. width*height = num pixels to raycast -- make sure no off-by-one-errors
    void load_res(std::string res);

    glm::vec3 ambient;
    void load_ambient(std::string ambient);

    void print();
};

#endif //CS410_CAMERA_H
