//
// Created by ikegentz on 9/13/18.
//

#ifndef CS410_DRIVER_H_H
#define CS410_DRIVER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "model.h"
#include "camera.h"
#include "light_source.h"
#include "image.h"
#include "sphere.h"
#include "raytracer_data.h"

/*
    Loads the driver file and grabs model data

*/
class Driver {
public:
    Driver(const char *driver_filename, RaytracerData* data);

    int read_driver_file();

    void apply_model_transformations();

private:
    std::unordered_map <std::string, WavefrontObj> wavefronts;
    const char *driver_filename;
    std::string output_dir;

    // keep track of global data for the raytracer
    RaytracerData* data;
    void add_model_from_str(std::string line);
};

#endif //CS410_DRIVER_H
