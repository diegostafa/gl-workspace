#ifndef window_cpp
#define window_cpp

#include "camera.cpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

class ambient {
public:
  glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f);
  float intensity = 1.0f;
};

class window_config {
public:
  const char *title;
  int w;
  int h;
  bool visible_mouse;
  glm::vec4 color;

  window_config(const char *title, int w, int h, bool visible_mouse = false,
                glm::vec4 color = glm::vec4(0.2, 0.2, 0, 0.3))
      : title(title), w(w), h(h), visible_mouse(visible_mouse), color(color){};

  float aspect() { return (float)w / (float)h; }
};

class time_manager {
public:
  float currentTick;
  float lastTick;
  float delta;

  void update(float newTick) {
    currentTick = newTick;
    delta = currentTick - lastTick;
    lastTick = currentTick;
  }
};

class window {
public:
  GLFWwindow *win;
  window_config cfg;
  ambient amb;
  camera cam;
  time_manager timer;

  window(const window_config &cfg, const camera &cam) : cfg(cfg), cam(cam) {
    win = create_window();
  }

  bool is_running() { return !glfwWindowShouldClose(win); }
  void swap_buffers() { glfwSwapBuffers(win); }
  void close() { glfwTerminate(); }
  void handle_time() { timer.update(glfwGetTime()); }

  GLFWwindow *create_window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    auto *window = glfwCreateWindow(cfg.w, cfg.h, cfg.title, NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();

    glEnable(GL_DEPTH_TEST);
    glClearColor(cfg.color.r, cfg.color.g, cfg.color.b, cfg.color.a);

    if (!cfg.visible_mouse)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
  }

  void handle_mouse() {
    double x, y;
    glfwGetCursorPos(win, &x, &y);
    cam.on_mouse_update(x, y);
  }

  void handle_events() {
    glfwPollEvents();
    handle_time();
    handle_mouse();
    key_handler();
  }

  void key_handler() {
    if (glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS)
      amb.intensity += 0.1f;
    if (glfwGetKey(win, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
      amb.intensity -= timer.delta;
    if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
      cam.speed = 8.0f * timer.delta;
    if (glfwGetKey(win, GLFW_KEY_SPACE) != GLFW_PRESS)
      cam.speed = 2.0f * 0.01;
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
      cam.move_to(cam.dir);
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
      cam.move_to(-cam.dir);
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
      cam.move_to(-glm::cross(cam.dir, cam.up));
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
      cam.move_to(glm::cross(cam.dir, cam.up));
  }
};

#endif