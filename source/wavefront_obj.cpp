#include "wavefront_obj.h"

WavefrontObj::add_vertex(float x, float y, float z)
{
  Vertex vert;
  vert.x = x;
  vert.y = y;
  vert.z = z;
  vertices.push_back(vert);
}
