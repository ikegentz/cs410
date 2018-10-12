//
// Created by ikegentz on 10/11/18.
//

#include "image.h"

Image::Image()
{
    this->bounds = glm::vec4(0,0,0,1);
    this->res = glm::vec2(16, 16);
}

Image::Image(const glm::vec4 &bounds, const glm::vec2 &res)
{
    this->bounds = glm::vec4(bounds);
    this->res = glm::vec2(res);
}

void Image::create_pixel_array()
{
    // create x rows of Pixels with y columns, default Pixel value
    this->pixel_array.resize(this->res.x, std::vector<Pixel>(this->res.y, Pixel()));
    std::cout << "Created image array of " << glm::to_string(this->res) << std::endl;
}