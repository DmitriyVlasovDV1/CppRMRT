#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinate;

uniform mat4 transform;
uniform mat4 viewProjection;
uniform float time;

out vec3 vertexColor;
out vec3 vertexPosition;
out vec3 vertexNormal;

// Main shader program function
void main() {
    gl_Position = transform * viewProjection * vec4(position.xyz, 1);
    vertexPosition = (transform * vec4(position.xyz, 1)).xyz;
    vertexColor = vec3(0);
    vertexNormal = mat3(transpose(inverse(transform))) * normal;
} // End of 'main' function