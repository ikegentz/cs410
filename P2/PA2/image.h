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
#include <sstream>
#include <fstream>
#include <utility>
#include <iomanip>      // std::setprecision

#include "pixel.h"
#include "camera.h"
#include "model.h"
#include "wavefront_obj.h"

class Image
{
public:
    Image(const glm::vec4 &bounds, const glm::vec2 &res);
    Image();
    glm::vec4 bounds;
    glm::vec2 res;

    std::vector<std::vector<Pixel>> pixel_array;
    void render_image(const Camera &camera, std::vector<Model> &models);
    void pixelPt(const unsigned i, const unsigned j, const double near,
                        const glm::vec3 &eye, const glm::vec3 &wv, const glm::vec3 &uv, const glm::vec3 &vv, Ray &ray);
    void write_image(const char* filename) const;
    unsigned bound_rgb(double in_color) const;
    // takes a pixel (containing the ray), and sets the pixels RGB and hit
    void ray_cast(Pixel &pixel, std::vector<Model> &models);
    // TODO colorme function here
};

#endif //CS410_IMAGE_H
