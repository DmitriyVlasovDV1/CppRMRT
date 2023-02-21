#version 460 core

out vec4 outColor;
in vec3 inColor;

/*****
 * Structures
 *****/

struct Material
{
    vec4 color;
};

struct Trace
{
    Material mtl;
    vec3 pos;
    vec3 norm;
    float t;
};

struct Sphere
{
    Material mtl;
    vec4 center;
    float radius;
};

struct Plane
{
    Material mtl;
    vec4 center;
    vec4 norm;
};

/*****
 * Buffers with figures
 *****/

layout(binding = 2, std430) buffer SphereBuffer
{
    Sphere spheres[];
} sphere_buffer;

/*****
 * SDF's
 *****/

float SDF_sphere(vec3 p, Sphere sphere)
{
    return length(p - sphere.center.xyz) - sphere.radius;
}

float SDF_plane(vec3 p, Plane plane)
{
    vec3 from_plane = p - plane.center.xyz;
    return dot(from_plane, plane.norm.xyz);
}




// Main shader program function
void main() {
    //outColor = vec4(inColor + vec3(1, 1, 1), 1);
    outColor = vec4(sphere_buffer.spheres[1].center.xyz, 1);

} // End of 'main' function
