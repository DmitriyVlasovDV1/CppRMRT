#version 460 core

out vec4 outColor;
in vec3 inColor;


/*****
 * Globals
 *****/

float max_dist = 20; // max ray traversal distance
float eps = 0.01;

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


float SDF_scene()
{
    float sdf = max_dist;
    for (int i = 0; i < sphere_buffer.spheres.length(); i++) {
        sdf = min(sdf, SDF_sphere(sphere_buffer.spheres[i]));
    }
    // TODO other figures

    return sdf;
}


Material trace(vec3 org, vec3 dir)
{
    float t = 0;
    while (t < max_dist)
    {
        vec3 pos = org + dir * t;
        float sdf = SDF_scene();

        if (sdf < eps)
        {
            Material res;
            res.color = vec4(1, 0, 0, 0);
            return res;
        }
        t += sdf;
    }
    Material res;
    res.color = vec4(0, 0, 0, 0);
    return res;
}


// Main shader program function
void main() {
    //outColor = vec4(inColor + vec3(1, 1, 1), 1);
    outColor = vec4(1, 0, 0, 1);
    vec3 cam_pos = vec3(0, 0, 0);
    vec3 cam_dir = vec3(0, 0, -1);
    float near = 0.1;
    //Material res = trace()

} // End of 'main' function
