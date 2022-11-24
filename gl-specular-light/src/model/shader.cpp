#pragma once

#include <string>
#include <glm/glm.hpp>

#include "../utils/shader_tools.h"

class shader {
public:
  unsigned int prog_id;
  shader(const char *vertex_path, const char *fragment_path) {
    prog_id = create_shader_program(vertex_path, fragment_path);
  }
  void use() const { glUseProgram(prog_id); }

  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(prog_id, name.c_str()), (int)value);
  }
  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(prog_id, name.c_str()), value);
  }
  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(prog_id, name.c_str()), value);
  }
  void setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(prog_id, name.c_str()), 1, &value[0]);
  }
  void setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(prog_id, name.c_str()), 1, &value[0]);
  }
  void setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(prog_id, name.c_str()), 1, &value[0]);
  }
  void setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(prog_id, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
  void setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(prog_id, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
  void setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(prog_id, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
};