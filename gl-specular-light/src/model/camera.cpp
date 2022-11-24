#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.cpp"

class camera {
public:
  glm::vec3 pos;
  glm::mat4 view;
  glm::vec3 up;
  glm::vec3 dir;
  glm::mat4 proj;
  float speed = 2.0f;
  float sensitivity = 0.2f;
  glm::vec2 coord;
  float yaw;
  float pitch;

  camera(float aspect, float fov = 90.0f, float min_zoom = 0.1f,
         float max_zoom = 100.0f)
      : pos(glm::vec3(0.0f, 0.0f, 0.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)),
        dir(glm::vec3(0.0f, 0.0f, 1.0f)),
        proj(glm::perspective(glm::radians(fov), aspect, min_zoom, max_zoom)){};

  void on_mouse_update(double new_x, double new_y) {
    float delta_x = new_x - coord.x;
    float delta_y = coord.y - new_y;
    coord.x = new_x;
    coord.y = new_y;
    delta_x *= sensitivity;
    delta_y *= sensitivity;
    rotate(delta_x, delta_y);
  }

  void update_view_matrix() { view = glm::lookAt(pos, pos + dir, up); }

  void move_to(glm::vec3 new_dir) {
    if (new_dir != glm::vec3(0)) {
      pos += glm::normalize(new_dir) * speed;
      update_view_matrix();
    }
  }

  void rotate(float y, float p) {
    yaw += y;
    pitch += p;
    if (pitch > +89.0f)
      pitch = +89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;

    dir = glm::normalize(
        glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                  sin(glm::radians(pitch)),
                  sin(glm::radians(yaw)) * cos(glm::radians(pitch))));

    update_view_matrix();
  }
};