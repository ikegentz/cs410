#include <vector>

// Represent a single vertex with X, Y, and Z
class Vertex
{
public:
    int x, y, z;
};

class WavefrontObj
{
public:
  std::std::vector<Vertex> vertices;

  void add_vertex(float x, float y, float z);
};
