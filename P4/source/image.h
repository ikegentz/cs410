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
    void process_pixel(Pixel &pixel);
    std::tuple<bool, double, glm::vec3, glm::vec3, glm::vec3, int>  ray_cast_model(const Ray& ray);

    glm::vec4 color_me(glm::vec3 intersection_point, Material &mat, const Pixel &pixel);
    glm::vec4 color_me_sphere(glm::vec3 intersection_point, Material &mat, const Pixel& pixel, const Sphere& sphere);
    void write_image(const char* filename) const;
    unsigned bound_rgb(double in_color) const;

};

#endif //CS410_IMAGE_H
