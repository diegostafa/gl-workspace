#pragma once

#include <glm/glm.hpp>

#include "mesh.cpp"
#include "shader.cpp"
#include "window.cpp"

class entity {
public:
  const mesh *data;
  glm::mat4 model = glm::mat4(1.0f);
  entity(mesh *data) : data(data) {};

  void move_to(glm::vec3 dest) {
    model = glm::translate(glm::identity<glm::mat4>(), dest);
  }
  void scale(glm::vec3 factor) {
    model = glm::scale(glm::identity<glm::mat4>(), factor);
  }
};