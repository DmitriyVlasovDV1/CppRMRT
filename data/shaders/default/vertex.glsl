#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Color;

out vec3 inColor;

// Main shader program function
void main() {
    gl_Position = vec4(Pos, 1);
    inColor = Color;
} // End of 'main' function