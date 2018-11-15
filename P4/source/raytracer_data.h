//
// Created by ikegentz on 11/14/18.
//

#ifndef P4_RAYTRACER_DATA_H
#define P4_RAYTRACER_DATA_H

class RaytracerData
{
public:
    Camera camera;
    std::vector<LightSource> light_sources;
    std::vector<Model> models;
    std::vector<Sphere> spheres;
    bool smooth;
};

#endif //P4_RAYTRACER_DATA_H
