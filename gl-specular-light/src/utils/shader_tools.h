#pragma once

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <GL/glew.h>

std::string load_shader(const char* filename);
GLuint compile_shader(const GLchar *shaderCode, GLenum shaderType);
GLuint create_shader_program(const char* vsFilename, const char* fsFilename);