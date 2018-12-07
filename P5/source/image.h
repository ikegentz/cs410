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
#include <cmath>


#include "pixel.h"
#include "camera.h"
#include "model.h"
#include "wavefront_obj.h"
#include "light_source.h"
#include "sphere.h"
#include "raytracer_data.h"

#include <tuple>
#include <thread>
#include <mutex>


class Image
{
public:
    Image(RaytracerData* data);
    Image();
    glm::vec4 bounds;
    glm::vec2 res;
    RaytracerData* data;
    std::vector<std::vector<Pixel>> pixel_array;

    void render_image();
    void pixelPt(const unsigned i, const unsigned j, const double near,
                        const glm::vec3 &eye, const glm::vec3 &wv, const glm::vec3 &uv, const glm::vec3 &vv);
    void write_image(const char* filename) const;
    unsigned bound_rgb(double in_color) const;

    static constexpr int MAX_THREADS = 8;

    static constexpr float AIR_ETA = 1;

};

#endif //CS410_IMAGE_H
