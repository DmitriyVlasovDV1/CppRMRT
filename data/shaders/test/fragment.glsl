#version 330 core
out vec4 outColor;

in vec3 inColor;

// Main shader program function
void main() {
    outColor = vec4(inColor, 1);
} // End of 'main' function