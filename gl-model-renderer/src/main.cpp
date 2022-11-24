#include "model/entity.cpp"
#include "model/shader.cpp"
#include "model/window.cpp"
#include <glm/trigonometric.hpp>

int main() {
  auto cfg = window_config("Opengl model renderer", 800, 800, true);
  auto cam = camera(cfg.aspect());
  auto win = window(cfg, cam);
  auto shad = shader("shaders/mod_vert.glsl", "shaders/mod_frag.glsl");
  auto e = entity("assets//backpack/backpack.obj");
  shad.use();

  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

  while (win.is_running()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    win.handle_events();
    glPointSize((glm::sin(win.timer.currentTick) + 2) * 10);

    e.scale(glm::vec3(glm::sin(win.timer.currentTick) + 10, 1, 1));
    shad.set_mat4("projection", win.cam.proj);
    shad.set_mat4("view", win.cam.view);
    shad.set_mat4("model", e.model);
    e.draw(shad);

    win.swap_buffers();
  }
  win.close();
}
