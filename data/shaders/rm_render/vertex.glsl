#version 460 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 Color;

out vec3 inColor;

// Main shader program function
void main() {
    gl_Position = vec4(inPos, 1);
    inColor = inPos;
} // End of 'main' function
