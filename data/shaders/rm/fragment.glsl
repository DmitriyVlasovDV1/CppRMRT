#version 460 core
#define PI 3.141592653589793

out vec4 outColor;
in vec3 inColor;

uniform float time;
uniform int frame_w;
uniform int frame_h;

/*****
 * Globals
 *****/

float max_dist = 20; // max ray traversal distance
float eps = 0.005;
vec3 light_dir = vec3(0, 1, 0);

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
    float radius;
};

struct Box
{
    Material mtl;
    float radius;
};

/*****
 * SSBO's
 *****/

layout(binding = 2, std430) buffer SphereBuffer
{
    Sphere spheres[];
} sphere_buffer;

layout(binding = 3, std430) buffer BoxBuffer
{
    Box boxes[];
} box_buffer;

layout(binding = 4, std430) buffer MatricesBuffer
{
    mat4 matrices[];
} matrices_buffer;

layout(binding = 5, std430) buffer FiguresBuffer
{
    int figures[];
} figures_buffer;


/*****
 * SDF's
 *****/

float unite(float a, float b)
{
    return min(a, b);
}

float inter(float a, float b)
{
    return max(a, b);
}

float sub(float a, float b)
{
    return max(a, -b);
}


float SDF_sphere(vec3 p, Sphere sphere)
{
    return length(p) - sphere.radius;
}


float SDF_box(vec3 p, Box box)
{
    vec3 q = abs(p) - vec3(box.radius);

    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float SDF_scene(vec3 pos)
{
    float sdf = max_dist;
    for (int i = 0; i < sphere_buffer.spheres.length(); i++) {
        vec3 pos1 = (inverse(matrices_buffer.matrices[0]) * vec4(pos, 1)).xyz;
        sdf = min(sdf, SDF_sphere(pos1, sphere_buffer.spheres[i]));
    }

    for (int i = 0; i < box_buffer.boxes.length(); i++) {
        sdf = min(sdf, SDF_box(pos, box_buffer.boxes[i]));
    }
    //sdf = min(max_dist, max(SDF_sphere(pos, sphere_buffer.spheres[0]), SDF_sphere(pos, sphere_buffer.spheres[1])));
    // TODO other figures

    return sdf;
}

Material average_mtl(vec3 pos)
{
    Material mtl;
    float sdf = SDF_scene(pos);

    for (int i = 0; i < sphere_buffer.spheres.length(); i++) {
        float k = SDF_sphere(pos, sphere_buffer.spheres[i]) / sdf;
        mtl.color += sphere_buffer.spheres[i].mtl.color * k;
    }
    for (int i = 0; i < box_buffer.boxes.length(); i++) {
        float k = SDF_box(pos, box_buffer.boxes[i]) / sdf;
        mtl.color += box_buffer.boxes[i].mtl.color * k;
    }
    return mtl;
}

/*****
 * Utils
 *****/

vec3 get_norm(vec3 pos, vec3 up)
{
    vec2 step = vec2(eps, 0);
    vec3 norm;
    /*
    for (int i = 0; i < 8; i++) {
        float angle = float(i) * PI * 0.125;
        vec3 vx = vec3(0, sin(angle), cos(angle));
        vec3 vy = vec3(sin(angle), 0, cos(angle));
        vec3 vz = vec3(sin(angle), cos(angle), 0);

        norm += vx * SDF_scene(pos + vx * eps) - vx * SDF_scene(pos - vx * eps);
        norm += vy * SDF_scene(pos + vy * eps) - vy * SDF_scene(pos - vy * eps);
        norm += vz * SDF_scene(pos + vz * eps) - vz * SDF_scene(pos - vz * eps);
    }
    */
    norm.x += SDF_scene(pos + step.xyy) - SDF_scene(pos - step.xyy);
    norm.y += SDF_scene(pos + step.yxy) - SDF_scene(pos - step.yxy);
    norm.z += SDF_scene(pos + step.yyx) - SDF_scene(pos - step.yyx);

    if (dot(up, norm) != 0) {
        return normalize(norm) * sign(dot(up, norm));
    }
    return normalize(norm);
}


Material trace(vec3 org, vec3 dir)
{
    float t = 0;
    while (t < max_dist)
    {
        vec3 pos = org + dir * t;
        float sdf = SDF_scene(pos);

        if (sdf < eps)
        {
            Material res;
            res = average_mtl(pos);
            res.color *= 0.4 * abs(dot(light_dir, get_norm(pos, -dir))) + 0.4;
            res.color = vec4(get_norm(pos, -dir), 1);
            return res;
        }
        t += sdf;
    }
    Material res;
    res.color = vec4(0, 0, 0, 1);
    return res;
}


// Main shader program function
void main() {
    //outColor = vec4(inColor + vec3(1, 1, 1), 1);
    vec3 cam_pos = normalize(vec3(cos(time * 0), 2, sin(time * 0))) * 4;
    vec3 cam_dir = normalize(vec3(0) - cam_pos);
    vec3 up = vec3(0, 1, 0);
    vec3 cam_right = normalize(cross(cam_dir, up));
    vec3 cam_up = normalize(cross(cam_right, cam_dir));
    float near = 0.5;

    vec3 pixel_pos = (cam_dir * near +
        cam_up * ((float(gl_FragCoord.y) / frame_h) - 0.5) +
        cam_right * ((float(gl_FragCoord.x) / frame_w) - 0.5));
    vec3 dir = normalize(pixel_pos);
    vec3 org = pixel_pos + cam_pos;

    float t = time * 2;
    light_dir = normalize(2 * vec3(0, 1, 0) + vec3(sin(t), 0, cos(t)));

    Material mtl;
    mtl = trace(org, dir);

    outColor = vec4(mtl.color);
    //outColor = vec4(0 * float(gl_FragCoord.x) / frame_w, float(gl_FragCoord.y) / frame_h, 0, 1);
    int i = int(float(gl_FragCoord.x) / frame_w * 4);
    int j = 3 - int(float(gl_FragCoord.y) / frame_h * 4);
    vec4 v = vec4(0, 0, 0, 1);
    //v = figure_buffer.matr * v;
    //outColor = v;
    //Material res = trace()

} // End of 'main' function
