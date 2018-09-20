//
// Created by ikegentz on 9/13/18.
//

#ifndef CS410_WAVEFRONT_OBJ_H
#define CS410_WAVEFRONT_OBJ_H

#include <vector>
#include <string>

// forward declarations
class Vertex;

class WavefrontObj
{
public:
  std::vector<Vertex> vertices;

  void add_vertex_from_str(const std::vector<std::string> &tokens);
  void add_vertex(float x, float y, float z);
  void load_wavefront_file(const char* filename);
};

// Represent a single vertex with X, Y, and Z
class Vertex
{
public:
    float x, y, z;

    Vertex();
    Vertex(float x, float y, float z);
};

#endif //CS410_MODEL_H
