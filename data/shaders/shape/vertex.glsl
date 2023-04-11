#version 330 core
layout (location = 0) in vec3 color;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 textureCoordinate;

uniform mat4 transformMatrix;
uniform mat4 viewProjection;
uniform float time;

out vec3 vertexPosition;
out vec3 vertexNormal;
out vec3 vertexColor;

// Main shader program function
void main() {
    vertexPosition = vec3(transformMatrix * vec4(position, 1));
    gl_Position = viewProjection * vec4(vertexPosition.xyz, 1);
    vertexNormal = mat3(transpose(inverse(transformMatrix))) * normal;
    vertexColor = color;
} // End of 'main' function