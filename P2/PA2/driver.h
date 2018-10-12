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

/*
    Loads the driver file and grabs each model's data
*/
class Driver
{
public:
    Driver(const char* driver_filename);
    void apply_model_transformations();
    void create_output_directory();
    void write_results();
    void generate_image_plane();

private:
    std::unordered_map<std::string, WavefrontObj> wavefronts;
    const char* driver_filename;
    std::string output_dir;

    std::vector<Model> models;
    std::vector<LightSource> light_sources;
    Camera camera;
    Image image;

    void add_model_from_str(std::string line);
};

#endif //CS410_DRIVER_H
