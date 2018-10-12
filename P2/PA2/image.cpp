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

void Image::create_pixel_array(const Camera &camera)
{
    // create y rows of Pixels with x columns, default Pixel value
    this->pixel_array.resize(this->res.y, std::vector<Pixel>(this->res.x, Pixel()));
    std::cout << "CREATED IMAGE ARRAY OF " << this->res.y << " ROWS x " << this->res.x << " COLS" << std::endl;
    std::cout << "SHOOTING PIXEL RAYS INTO SCENE..." << std::endl;

    double left = this->bounds.x;
    double bottom = this->bounds.y;
    double right = this->bounds.z;
    double top = this->bounds.w;

    std::cout << "left: " << left << ", bottom: " << bottom << ", right: " << right << ", top: " << top<< std::endl;

    glm::vec3 wv = camera.eye - camera.look;
    wv = glm::normalize(wv);
    glm::vec3 uv = glm::cross(camera.up, wv);
    uv = glm::normalize(uv);
    glm::vec3 vv = glm::cross(wv, uv);
    vv = glm::normalize(vv); // TODO Does this need to be normalized????

    //we will be looping through rows, so go through vertically
    for(unsigned row = 0; row < this->res.y; ++row)
    {
        for(unsigned col = 0; col < this->res.x; ++col)
        {
            this->pixelPt(row, col, -camera.d, camera.eye, wv, uv, vv, this->pixel_array[row][col].ray);
        }
    }
    this->pixel_array[0][0].ray.print();

}

void Image::pixelPt(const unsigned i, const unsigned j, const double near,
        const glm::vec3 &eye, const glm::vec3 &wv, const glm::vec3 &uv, const glm::vec3 &vv, Ray &ray)
{
    float width = res.x;
    float height = res.y;
    double left = this->bounds.x;
    double bottom = this->bounds.y;
    double right = this->bounds.z;
    double top = this->bounds.w;

    double px = i/(width-1)*(right-left)+left;
    double py = j/(height-1)*(bottom-top) + top;

    glm::vec3 pixpt = eye + (wv * float(near)) + (uv * float(px)) + (vv * float(py)); // the ray's position
    glm::vec3 shoot = pixpt - eye;
    glm::normalize(shoot); // the ray's direction
    ray.position = pixpt;
    ray.set_direction(shoot);
}
