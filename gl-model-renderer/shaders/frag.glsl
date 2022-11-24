#version 450 core

in vec3 px_pos;
in vec3 px_norm;
in vec2 px_tex;

out vec4 px_color;

uniform sampler2D texture_sample;
uniform vec3 ambient_color;
uniform vec3 diffuse_source;
uniform float ambient_intensity;
uniform float diffuse_intensity;
uniform vec3 camera_pos;

void main() {
    vec3 camera_direction = normalize(camera_pos - px_pos);
    
    // ambient light
    vec3 ambient = ambient_color * ambient_intensity;
    
    // diffuse light
    vec3 norm = normalize(px_norm);
    vec3 diffuse_direction = normalize(diffuse_source - px_pos);
    vec3 diffuse_reflection = reflect(-diffuse_direction, norm);
    vec3 diffuse = max(dot(norm, diffuse_direction), 0.0) * ambient_color;

    // spcular reflection on the surface
    float specular_area = pow(max(dot(camera_direction, diffuse_reflection), 0.0), 32);
    vec3 specular = diffuse_intensity * specular_area * ambient_color;
    
    // piping
    px_color = texture(texture_sample, px_tex) * vec4(ambient + diffuse + specular, 1.0f);
} 
