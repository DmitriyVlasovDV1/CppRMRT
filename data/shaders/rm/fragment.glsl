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

float max_dist = 4; // max ray traversal distance
float eps = 0.0001;
vec3 light_dir = vec3(1, 1, 1);


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

struct Surface {
    Material mtl;
    float sdf;
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

/*****
 * SDF's
 *****/

// power smooth min (k = 8);
float smin( float a, float b, float k )
{
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}

Surface unite(Surface a, Surface b)
{
    Surface res;
    res.sdf = min(a.sdf, b.sdf);
    if (a.sdf < b.sdf) {
        res.mtl = a.mtl;
        return res;
    }
    res.mtl = b.mtl;
    return res;
}

Surface inter( Surface a, Surface b )
{
    if (a.sdf < b.sdf) {
        return b;
    }
    return a;
}

Surface sub( Surface a, Surface b )
{
    Surface res;
    res.mtl = a.mtl;
    res.sdf = max(a.sdf, -b.sdf);
    if (a.sdf > -b.sdf)  {
        res.sdf = a.sdf;
        return res;
    }
    res.sdf = -b.sdf;
    return res;
}

Surface SDF_sphere(vec3 p, mat4 matr, Sphere sphere)
{
    vec3 pos_matr = (matr * vec4(p, 1)).xyz;
    Surface res;
    res.sdf = length(pos_matr) - sphere.radius;
    res.mtl = sphere.mtl;
    return res;
}


Surface SDF_box(vec3 p, mat4 matr, Box box)
{
    vec3 pos_matr = (matr * vec4(p, 1)).xyz;
    Surface res;
    vec3 q = abs(pos_matr) - vec3(box.radius);

    res.sdf = length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
    res.mtl = box.mtl;
    return res;
}


Surface SDF_scene(vec3 pos)
{
Surface res;
Sphere sph;
sph.radius = 0.1;res.sdf = max_dist;
res = sub(sub(sub(sub(sub(sub(sub(sub(sub(sub(sub(sub(SDF_box(pos, matrices_buffer.matrices[0] * mat4(1), box_buffer.boxes[0]), SDF_sphere(pos, matrices_buffer.matrices[1] * mat4(1), sph)), SDF_sphere(pos, matrices_buffer.matrices[2] * mat4(1), sph)), SDF_sphere(pos, matrices_buffer.matrices[3] * mat4(1), sph)), SDF_sphere(pos, matrices_buffer.matrices[4] * mat4(1), sph)), SDF_sphere(pos, matrices_buffer.matrices[5] * mat4(1), sph)), SDF_sphere(pos, matrices_buffer.matrices[6] * mat4(1), sph)), SDF_sphere(pos, matrices_buffer.matrices[7] * mat4(1), sph)), SDF_sphere(pos, matrices_buffer.matrices[8] * mat4(1), sph)), SDF_sphere(pos, matrices_buffer.matrices[9] * mat4(1), sph)), SDF_sphere(pos, matrices_buffer.matrices[10] * mat4(1), sph)), SDF_sphere(pos, matrices_buffer.matrices[11] * mat4(1), sph)), SDF_sphere(pos, matrices_buffer.matrices[12] * mat4(1), sph));
return res;
}

/*****
 * Utils
 *****/

vec3 get_norm(vec3 pos, vec3 up)
{
    vec2 step = vec2(0.01 * eps, 0);
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
    norm.x += SDF_scene(pos + step.xyy).sdf - SDF_scene(pos - step.xyy).sdf;
    norm.y += SDF_scene(pos + step.yxy).sdf - SDF_scene(pos - step.yxy).sdf;
    norm.z += SDF_scene(pos + step.yyx).sdf - SDF_scene(pos - step.yyx).sdf;

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
        Surface srf = SDF_scene(pos);

        if (srf.sdf < eps)
        {
            //res = average_mtl(pos);
            srf.mtl.color *= 0.4 * abs(dot(light_dir, get_norm(pos, -dir))) + 0.4;
            srf.mtl.color = vec4(get_norm(pos, -dir), 1);
            //res.color = vec4(1, 0, 0, 1);
            return srf.mtl;
        }
        t += srf.sdf;
    }
    Material res;
    res.color = vec4(0.1, 0, 0.2, 1);
    return res;
}


// Main shader program function
void main() {
    //outColor = vec4(inColor + vec3(1, 1, 1), 1);
    vec3 cam_pos = normalize(vec3(abs(cos(time * 1 + 1)), 0.7, abs(sin(time * 1 + 1)))) * 1;
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
    outColor = vec4(0);

    Material mtl;
    mtl = trace(org, dir);
    outColor = vec4(mtl.color);
    //outColor = vec4(0 * float(gl_FragCoord.x) / frame_w, float(gl_FragCoord.y) / frame_h, 0, 1);
    int i = int(float(gl_FragCoord.x) / frame_w * 4);
    int j = 3 - int(float(gl_FragCoord.y) / frame_h * 4);
    vec4 v = vec4(0, 0, 0, 1);
    //outColor = vec4(matrices_buffer.matrices[2][j][i], 0, 0, 1);
    //v = figure_buffer.matr * v;
    //outColor = sphere_buffer.spheres[0].mtl.color;
    //Material res = trace()''

} // End of 'main' function
