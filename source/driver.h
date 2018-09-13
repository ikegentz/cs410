//
// Created by ikegentz on 9/13/18.
//

#ifndef CS410_DRIVER_H_H
#define CS410_DRIVER_H

#include <string>
#include <vector>
#include "model.h"

class Driver
{
public:
    Driver(char* driver_filename);
private:
    std::vector<Model> models;

};

#endif //CS410_DRIVER_H
