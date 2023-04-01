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


#define code_E 69
#define code_S 83
#define code_m 109
#define code_b 98
#define code_s 115
#define code_I 73
#define code_U 85
#define code_M 77

float max_dist = 10; // max ray traversal distance
float eps = 0.001;
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

/*****
 * SSBO's
 *****/

uniform mat4 matrs[100];

layout(binding = 2, std430) buffer SphereBuffer
{
    Sphere spheres[];
} sphere_buffer;

layout(binding = 3, std430) buffer BoxBuffer
{
    Box boxes[];
} box_buffer;

struct mystr {
    float a00, a01, a02, a03;
    float a10, a11, a12, a13;
    float a20, a21, a22, a23;
    float a30, a31, a32, a33;
};

layout(binding = 4, std430) buffer MatricesBuffer
{
    mat4 matrices[];
} matrices_buffer;

layout(binding = 5, std430) buffer FiguresBuffer
{
    int figures[];
} figures_buffer;

struct Surface {
    Material mtl;
    float sdf;
};

layout(binding = 6, std430) buffer SymbolsBuffer
{
    Surface symbols[];
} symbols_buffer;

/*****
 * SDF's
 *****/


Surface unite(Surface a, Surface b)
{
    if (a.sdf < b.sdf) {
        return a;
    }
    return b;
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
    if (a.sdf > -b.sdf)  {
        res.sdf = a.sdf;
        return res;
    }
    res.sdf = -b.sdf;
    return res;
}

Surface SDF_sphere(vec3 p, Sphere sphere)
{
    Surface res;
    res.sdf = length(p) - sphere.radius;
    res.mtl = sphere.mtl;
    return res;
}


Surface SDF_box(vec3 p, Box box)
{
    Surface res;
    vec3 q = abs(p) - vec3(box.radius);

    res.sdf = length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
    res.mtl = box.mtl;
    return res;
}


Surface symbols[128];
int operations[5];
int len = figures_buffer.figures.length();

Surface SDF_scene(vec3 pos)
{
    float sdf = max_dist;

    mat4 cur_matr = mat4(1);
    int symb_pos = 0;
    int op_pos = 0;
    int matr_pos = 0;

    Surface tmp;
    int i = 0;
    while (i < len) {
        int code = figures_buffer.figures[i];
        int ind;
        vec3 pos_matr;
        switch (code) {
        case code_m:
            ind = figures_buffer.figures[i + 1];
            i++;
            cur_matr *= matrices_buffer.matrices[ind];
            break;
        case code_s:
            ind = figures_buffer.figures[i + 1];
            i++;
            pos_matr = (cur_matr * vec4(pos, 1)).xyz;
            symbols[symb_pos] = SDF_sphere(pos_matr, sphere_buffer.spheres[ind]);
            symb_pos++;
            break;
        case code_b:
            ind = figures_buffer.figures[i + 1];
            i++;
            pos_matr = (cur_matr * vec4(pos, 1)).xyz;
            symbols[symb_pos] = SDF_box(pos_matr, box_buffer.boxes[ind]);
            symb_pos++;
            break;
        case code_S:
            operations[op_pos] = code_S;
            op_pos++;
            break;
            case code_I:
            operations[op_pos] = code_I;
            op_pos++;
            break;
        case code_U:
            symb_pos--;
            Surface prev = symbols[symb_pos];
            while (symbols[symb_pos - 1].sdf != -max_dist)
            {
                Surface b = symbols[symb_pos];
                symb_pos--;
                Surface a = symbols[symb_pos];
                symbols[symb_pos] = unite(a, b);
            }

            return symbols[symb_pos];
            //symbols[symb_pos - 1] = prev; ///  WTF???
            break;
        case code_E:
            Surface srf;
            srf.sdf = -max_dist;
            symbols[symb_pos] = srf;
            symb_pos++;
            break;
        }
        i++;
    }


    return symbols[0];
}

/*
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
*/

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
            //srf.mtl.color = vec4(get_norm(pos, -dir), 1);
            //res.color = vec4(1, 0, 0, 1);
            return srf.mtl;
        }
        t += srf.sdf;
    }
    Material res;
    res.color = vec4(0, 0, 0, 1);
    return res;
}


// Main shader program function
void main() {
    //outColor = vec4(inColor + vec3(1, 1, 1), 1);
    vec3 cam_pos = normalize(vec3(cos(time * 0 + 1), 1, sin(time * 0 + 1))) * 2;
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
