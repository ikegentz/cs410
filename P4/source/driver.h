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

namespace Raytracer {

    /*
        Loads the driver file and grabs model data

    */
    class Driver {
    public:
        Driver(const char *driver_filename);

        int read_driver_file();

        void apply_model_transformations();

        void generate_image(const char *filename);

    private:
        std::unordered_map <std::string, WavefrontObj> wavefronts;
        const char *driver_filename;
        std::string output_dir;
        unsigned recurse_depth;

        std::vector <Model> models;
        std::vector <LightSource> light_sources;
        std::vector <Sphere> spheres;
        Camera camera;
        Image image;

        void add_model_from_str(std::string line);
    };
}

#endif //CS410_DRIVER_H
