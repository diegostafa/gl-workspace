#ifndef mesh_cpp
#define mesh_cpp

#include "shader.cpp"
#include "texture.cpp"
#include "vertex.cpp"
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class mesh {
public:
  unsigned int vao, vbo, ebo;
  std::vector<vertex> vertices;
  std::vector<texture> textures;
  std::vector<unsigned int> indices;

  mesh(const std::vector<vertex> &v, const std::vector<texture> &t,
       const std::vector<unsigned int> &i)
      : vertices(v), textures(t), indices(i) {
    pass_data();
  }

  void pass_data() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                          (void *)offsetof(vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                          (void *)offsetof(vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
                          (void *)offsetof(vertex, tex_coord));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
  }
  
  void configure_shader(shader &s) {
    auto diffuse_counter = 1;
    auto specular_counter = 1;
    auto normal_counter = 1;
    std::string number;

    for (unsigned int i = 0; i < textures.size(); i++) {
      glActiveTexture(GL_TEXTURE0 + i);
      auto name = textures[i].type;

      if (name == "texture_diffuse")
        number = std::to_string(diffuse_counter++);
      else if (name == "texture_specular")
        number = std::to_string(specular_counter++);
      else if (name == "texture_normal")
        number = std::to_string(normal_counter++);

      glUniform1i(glGetUniformLocation(s.prog_id, (name + number).c_str()), i);
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
  }

  void render(shader &s) {
    configure_shader(s);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()),
                   GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
  }
};
#endif