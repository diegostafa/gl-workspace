#version 450 core

layout (location = 0) in vec3 vert_pos;
in vec3 vert_norm;
in vec2 vert_tex;

out vec3 px_pos;
out vec3 px_norm;
out vec2 px_tex;

uniform mat4 model;
uniform mat4 mvp;

void main() {
    gl_Position = mvp*vec4(vert_pos, 1.0f);

    // piping
    px_pos = vec3(model * vec4(vert_pos, 1.0));
    px_norm = vert_norm;
    px_tex = vert_tex;
}