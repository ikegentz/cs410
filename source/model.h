//
// Created by ikegentz on 9/13/18.
//

#ifndef CS410_MODEL_H
#define CS410_MODEL_H

#include <string>

class Model
{
public:
    float wx, wy, wz;
    float theta;
    float scale;
    float tx, ty, tz;
    std::string wavefront_filename;

    std::string to_string();
};

#endif //CS410_MODEL_H