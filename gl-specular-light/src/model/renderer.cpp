#pragma once

#define STB_IMAGE_IMPLEMENTATION // needed for stb include

#include "entity.cpp"
#include <GLFW/glfw3.h>
#include "mesh.cpp"
#include "shader.cpp"
#include "window.cpp"
#include <glm/glm.hpp>
#include <stb/stb_image.h>

class renderer {
public:
  window *surface;
  shader *shad;
  renderer(window *win, shader *shad) : surface(win), shad(shad) {
    shad->use();
  }

  void set_data(const mesh &data) {
    GLuint cube_vao, cube_vertex_vbo, cube_tex_vbo, cube_texture;

    // generate buffer id
    glGenVertexArrays(1, &cube_vao);
    glGenBuffers(1, &cube_vertex_vbo);
    glGenBuffers(1, &cube_tex_vbo);
    glGenTextures(1, &cube_texture);
    glBindVertexArray(cube_vao);

    // pass vertex data (position/normal)
    glBindBuffer(GL_ARRAY_BUFFER, cube_vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data.vertices), 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data.vertices), data.vertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));

    // pass texture coord
    glBindBuffer(GL_ARRAY_BUFFER, cube_tex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data.tex), 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data.tex), data.tex);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // enable shader registers
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // pass texture data
    if (data.texture_img) {
      stbi_uc *img_data;
      int img_w, img_h;
      img_data = stbi_load(data.texture_img, &img_w, &img_h, NULL, 0);
      glBindTexture(GL_TEXTURE_2D, cube_texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_w, img_h, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, img_data);
      glGenerateMipmap(GL_TEXTURE_2D);
      stbi_image_free(img_data);
    }
  }

  glm::vec3 orbiting_coord() {
  return glm::vec3(glm::cos(glfwGetTime()) * 3,
                   glm::cos(glfwGetTime()) * 0,
                   glm::sin(glfwGetTime()) * 3);
}
  void draw_frame(entity &e) {
    auto mvp = surface->cam->proj * surface->cam->view * e.model;
    shad->setVec3("ambient_color", surface->amb.color);
    shad->setVec3("diffuse_source", orbiting_coord());
    shad->setFloat("ambient_intensity", surface->amb.intensity);
    shad->setFloat("diffuse_intensity", 1.0f);
    shad->setVec3("camera_pos", surface->cam->pos);
    shad->setMat4("model", e.model);
    shad->setMat4("mvp", mvp);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
};
