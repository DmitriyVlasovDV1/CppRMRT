#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexColor;

uniform vec3 cameraPosition;
uniform vec3 cameraDirection;
uniform float time;
uniform int numberOfDemo;

vec3 lightColor = vec3(0.7);
vec3 lightDirection = -vec3(1, 3, 3);

out vec4 outColor;

vec3 lightResponse() {
    vec3 V = normalize(vertexPosition - cameraPosition), L = normalize(lightDirection);
    vec3 normal = normalize(vertexNormal);
    vec3 RV = normalize(reflect(V, normal)), H = normalize(V - L);

    vec3 ambientOcculusion = vec3((1 - abs(dot(RV, normal))) / 5);
    float diffuse = max(pow((dot(normal, -L) + 1) / 1.7, 2), 0.15);
    float pointColorWeight = 2.5, lightColorWeight = 1;
    float summaryWeight = pointColorWeight + lightColorWeight;
    vec3 pointRawColor = vertexColor * pointColorWeight;
    vec3 lightRawColor = lightColor * lightColorWeight;
    vec3 pointNaturalColor = vec3(max(min((pointRawColor.x + lightRawColor.x) / summaryWeight, 1.0), 0.0),
                                  max(min((pointRawColor.y + lightRawColor.y) / summaryWeight, 1.0), 0.0),
                                  max(min((pointRawColor.z + lightRawColor.z) / summaryWeight, 1.0), 0.0));
    return pointNaturalColor * diffuse;
}

// Main shader program function
void main() {
    float depth = 1 / length(cameraPosition - vertexPosition) * 50;
    depth = min(pow(depth, 1.2), 1.0);
    if (abs(numberOfDemo) == 0)
        outColor = vec4(lightResponse() * depth, 1);
    else if (abs(numberOfDemo) == 1)
        outColor = vec4(normalize(vertexNormal), 1);
    else if (abs(numberOfDemo) == 2)
        outColor = vec4(vertexPosition, 1);
    else if (abs(numberOfDemo) == 3)
        outColor = vec4(vertexColor, 1);
} // End of 'main' function