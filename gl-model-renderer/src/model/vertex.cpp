#ifndef vertex_cpp
#define vertex_cpp

#include <glm/glm.hpp>

class vertex {
public:
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coord;
  glm::vec3 tangent;
  glm::vec3 bi_tangent;
  glm::vec3 height;
};

#endif