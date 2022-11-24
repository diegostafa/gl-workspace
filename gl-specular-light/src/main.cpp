#include "model/entity.cpp"
#include "model/renderer.cpp"
#include "model/shader.cpp"
#include "model/window.cpp"

glm::vec3 orbiting_coord() {
  return glm::vec3(glm::cos(glfwGetTime()) * 3, glm::cos(glfwGetTime()) * 0,
                   glm::sin(glfwGetTime()) * 3);
}

int main() {
  auto cfg = window_config("Opengl cubes", 800, 800);
  auto win = window(cfg);
  auto cam = camera(cfg.aspect());
  auto msh = mesh("assets/container.jpg");
  auto cube1 = entity(&msh);
  auto cube2 = entity(&msh);
  auto shad = shader("shaders/vert.glsl", "shaders/frag.glsl");
  auto rend = renderer(&win, &shad);
  win.cam = &cam;
  rend.set_data(msh);

  while (win.is_running()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    win.handle_events();

    cube1.move_to(orbiting_coord());

    rend.draw_frame(cube1);
    rend.draw_frame(cube2);
    win.swap_buffers();
  }
  win.close();
}
