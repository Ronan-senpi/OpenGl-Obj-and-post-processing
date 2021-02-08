#version 430 core

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normals;

layout(location = 1) uniform mat4 model; // model to world
layout(location = 6) uniform mat4 view; // model to world
layout(location = 7) uniform mat4 projection; // model to world

out vec3 fragPos;
out vec3 normals;

void main() {

    fragPos = vec3(model * vec4(in_vertex, 1.0));
    normals = vec3(transpose(inverse(model))) * in_normals;

    gl_Position = projection * view * vec4(fragPos, 1.0);
//
//    normals = vec3(transpose(inverse(model))) * in_normals;
}