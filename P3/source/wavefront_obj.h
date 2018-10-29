//
// Created by ikegentz on 9/13/18.
//

#ifndef CS410_WAVEFRONT_OBJ_H
#define CS410_WAVEFRONT_OBJ_H

#define ERROR 0.00001f

#include <vector>
#include <string>

// forward declarations
class Vertex;
class Face;

class WavefrontObj
{
public:
  std::vector<Vertex> vertices;
  std::vector<Face> faces;
  std::string smoothing;

  void add_vertex_from_str(const std::vector<std::string> &tokens);
  void add_vertex(float x, float y, float z);
  void add_face_from_str(const std::vector<std::string> &tokens);
  void add_face(int v1, int vn1, int v2, int vn2, int v3, int vn3);
  void load_wavefront_file(const char* filename);
  void write_wavefront_file(const char* filename);
  
  static float fix_neg_zero(float in);

  void print();

private:
    std::pair<int, int> extract_face_values(const std::string &token);
};

// Represent a single vertex with X, Y, and Z
class Vertex
{
public:
    float x, y, z;

    Vertex();
    Vertex(float x, float y, float z);
    std::string to_string();
};

// Represents a face, defined by vertices
class Face
{
public:
    int v1, vn1, v2, vn2, v3, vn3; // vertex indices and vertex normals

    Face(int v1, int vn1, int v2, int vn2, int v3, int vn3);
    Face(const Face& face);
    std::string to_string();
};

#endif //CS410_MODEL_H
