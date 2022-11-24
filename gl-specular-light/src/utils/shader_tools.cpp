#include "shader_tools.h"

std::string load_shader(const char* filename) {
    std::string shaderCode;
    std::ifstream file(filename, std::ios::in);

    if (!file.good()) {
        std::cout << "Can't read file " << filename << std::endl;
        exit(1);
    }

    file.seekg(0, std::ios::end);
    shaderCode.resize((unsigned int)file.tellg());

    file.seekg(0, std::ios::beg);
    file.read(&shaderCode[0], shaderCode.size());
    file.close();

    return shaderCode;
}

GLuint compile_shader(const GLchar* shaderCode, GLenum shaderType) {
    GLuint shaderObject = glCreateShader(shaderType);

    if(shaderObject == 0) {
        std::cout << "Error creating shader type: " << shaderType << std::endl;
        exit(1);
    }

    glShaderSource(shaderObject, 1, &shaderCode, NULL);
    glCompileShader(shaderObject);

    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
        std::cout << "Error compiling shader type: " << shaderType << std::endl;
        std::cout << infoLog << std::endl;
        exit(1);
    }

    return shaderObject;
}

GLuint create_shader_program(const char *vsFilename, const char *fsFilename) {
    GLuint shaderProgram = glCreateProgram();

    std::string shaderStr = load_shader(vsFilename);
    GLuint vertexShader = compile_shader(shaderStr.c_str(), GL_VERTEX_SHADER);
    shaderStr = load_shader(fsFilename);
    GLuint fragmentShader = compile_shader(shaderStr.c_str(), GL_FRAGMENT_SHADER);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    GLchar infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Link Error: " << infoLog << std::endl;
        exit(1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

