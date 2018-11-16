//
// Created by ikegentz on 10/11/18.
//

#include "pixel.h"
#include "ray.h"

Pixel::Pixel()
{
    this->ray = Ray();
    this->rgba = glm::vec4(0, 0, 0, 1);
}
void Pixel::print() const
{
    std::cout << "PIXEL\n" << std::endl;
    ray.print();
    std::cout << "\t" << "(R, G, B, A): " << glm::to_string(this->rgba) << std::endl;
}