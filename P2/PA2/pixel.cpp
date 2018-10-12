//
// Created by ikegentz on 10/11/18.
//

#include "pixel.h"
#include "ray.h"

Pixel::Pixel()
{
    this->ray = Ray();
    this->hit = false;
    this->rgba = glm::vec4(0,0,0,1);
    this->last_t = -1;
    this->face_index = -1;
}

void Pixel::print() const
{
    std::cout << "PIXEL\n" <<
    "\t";
    ray.print();
    std::cout << "\tHit: " << this->hit << "\n" <<
    "\t" << "(R, G, B, A): " << glm::to_string(this->rgba) << "\n" <<
    "\t" << "Face Index: " << this->face_index << "\n" <<
    "\t" << "Latest t: " << this->last_t << std::endl;
}