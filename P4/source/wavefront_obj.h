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
  //TODO vector of Vertex Textures, need to actually read in VT

  std::string smoothing;

  void add_vertex_from_str(const std::vector<std::string> &tokens);
  void add_vertex(float x, float y, float z);
  void add_face_from_str(const std::vector<std::string> &tokens);
  void add_face(int v1, int vt1, int vn1, int v2, int vt2, int vn2, int v3, int vt3, int vn3);
  std::string load_wavefront_file(const char* filename); // returns the material filename
  void write_wavefront_file(const char* filename);
  
  static float fix_neg_zero(float in);

  void print();

private:
    // TODO VT's will be -1 from extract_face_values() if there is no VT set
    std::tuple<int, int, int> extract_face_values(const std::string &token);
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
    int v1, vt1, vn1, v2, vt2, vn2, v3, vt3, vn3; // vertex indices, texture index, and vertex normals

    Face(int v1, int vt1, int vn1, int v2, int vt2, int vn2, int v3, int vt3, int vn3);
    Face(const Face& face);
    std::string to_string();
};

#endif //CS410_MODEL_H
