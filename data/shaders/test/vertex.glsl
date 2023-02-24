#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Color;

out vec3 inColor;

uniform float time;

// Main shader program function
void main() {
    gl_Position = vec4(Pos.x, Pos.y + ((sin(time * Color.x * 4) / 10) + 0.1), Pos.z, 1);
    inColor = Color;
} // End of 'main' function