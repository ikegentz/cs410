//
// Created by ikegentz on 10/11/18.
//

#ifndef CS410_IMAGE_H
#define CS410_IMAGE_H

// Let us use some cool string casts
#define GLM_ENABLE_EXPERIMENTAL
// For un-named structs (make warnings go away since we know GLM works fine)
#define GLM_FORCE_SILENT_WARNINGS

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"
#include <string>
#include <vector>
#include <iostream>

#include "pixel.h"
#include "camera.h"

class Image
{
public:
    Image(const glm::vec4 &bounds, const glm::vec2 &res);
    Image();
    glm::vec4 bounds;
    glm::vec2 res;

    // create the big array of pixels to shoot the rays

    std::vector<std::vector<Pixel>> pixel_array;
    void create_pixel_array(const Camera &camera);
    void pixelPt(const unsigned i, const unsigned j, const double near,
                        const glm::vec3 &eye, const glm::vec3 &wv, const glm::vec3 &uv, const glm::vec3 &vv, Ray &ray);

    // TODO raycast function here
    // TODO colorme function here
};

#endif //CS410_IMAGE_H
