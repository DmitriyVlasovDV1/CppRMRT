#version 460 core
#define PI 3.141592653589793

out vec4 outColor;
in vec3 inColor;

uniform float time;
uniform int frame_w;
uniform int frame_h;
uniform vec3 cam_pos;
uniform vec3 cam_dir;
uniform vec3 cam_up;
uniform vec3 cam_right;
/*
vec3 cam_pos = vec3(0, 0, 10);
vec3 cam_dir = vec3(0, 0, -1);
vec3 cam_up = vec3(0, 1, 0);
vec3 cam_right = vec3(-1, 0, 0);
*/

/*****
 * Globals
 *****/

float max_dist = 50; // max ray traversal distance
float eps = 0.0001;
vec3 light_dir = vec3(1, 1, 1);
vec3 lightColor = vec3(0.7);
vec3 lightDirection = -vec3(1, 3, 3);


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
    float radius;
};

struct Box
{
    float radius;
};

struct Surface {
    Material mtl;
    float sdf;
};

struct Twist {
    vec4 cen;
    vec4 dir;
    float intensity;
};

struct Bend {
    vec4 cen;
    vec4 dir;
    vec4 rad;
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

layout(binding = 5, std430) buffer TwistBuffer
{
    Twist twists[];
} twist_buffer;

layout(binding = 6, std430) buffer BendBuffer
{
    Bend bends[];
} bend_buffer;
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
    return res;
}

Surface SDF_sphere(vec4 p, Sphere sphere)
{
    vec3 pos = p.xyz;
    Surface res;
    res.sdf = length(pos) - sphere.radius;
    //res.mtl = sphere.mtl;
    return res;
}




Surface SDF_box(vec4 p, Box box)
{
    vec3 pos = p.xyz;
    /*float c = cos(pos_matr.x);
    float s = sin(pos_matr.x);
    mat2  m = mat2(c,-s,s,c);
    vec3  pp = vec3(pos_matr.x,m*pos_matr.yz);*/
    Surface res;
    vec3 q = abs(pos) - vec3(box.radius / 2);

    res.sdf = length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
    //res.mtl = box.mtl;
    return res;
}

vec3 c_ = vec3(cos(time) * 4, 2 * sin(time), 0 );
vec3 dir_ = vec3(0, 0, 1);
vec3 rad_ = normalize(-c_);//normalize(vec3(0, 0, -1));
float d_ = 1;

mat4 rotateRad(float angle, vec3 axis) {
    axis = normalize(axis);
    const float sine = sin(angle), cosine = cos(angle),
    cosine1 = 1 - cosine;

    return mat4(
    cosine + axis.x * axis.x * cosine1,
    axis.x * axis.y * cosine1 + axis.z * sine,
    axis.x * axis.z * cosine1 - axis.y * sine, 0,
    axis.y * axis.x * cosine1 - axis.z * sine,
    cosine + axis.y * axis.y * cosine1,
    axis.y * axis.z * cosine1 + axis.x * sine, 0,
    axis.z * axis.x * cosine1 + axis.y * sine,
    axis.z * axis.y * cosine1 - axis.x * sine,
    cosine + axis.z * axis.z * cosine1, 0, 0, 0, 0, 1);
}

vec4 bend(vec4 p, mat4 matr, Bend bnd) {
    vec3 cen = (matr * bnd.cen).xyz;
    vec3 dir = (matr * normalize(bnd.dir)).xyz;
    vec3 rad = (matr * normalize(bnd.rad)).xyz;
    vec3 right = cross(rad, dir);

    mat4 m = rotateRad(atan(dot(p.xyz - cen, right), dot(p.xyz - cen, rad)), dir);
    return m * p;
}

vec4 twist(vec4 p, mat4 matr, Twist tw) {
    vec3 cen = (matr * tw.cen).xyz;
    vec3 dir = (matr * normalize(tw.dir)).xyz;

    mat4 m = rotateRad(dot(p.xyz - cen, dir) * tw.intensity, dir);
    return m * p;
}
/*
vec3 twist1(vec3 p, mat4 matr) {
    vec3 pos = (matr * vec4(p, 1)).xyz;

    vec3 c = (matr * vec4(c_, 1)).xyz;
    vec3 dir = (matr * vec4(dir_, 0)).xyz;
   vec3 rad = (matr * vec4(rad_, 0)).xyz;

    //vec3 pos = p;
    vec3 c = c_;
    vec3 dir = dir_;
    vec3 rad = rad_;
    vec3 right = cross(rad, dir);

    vec3 dlt = (matr * vec4(p - c_, 1)).xyz;
    mat4 m = rotateRad(atan(dot(pos - c, right), dot(pos - c, rad)), dir);

    vec3 a = (m * vec4(pos, 1)).xyz;
    vec3 res = a;//dir * dot(dir, pos) + a + b;
    return res;
}
*/

/*
Surface SDF_scene1(vec3 pos)
{
    Surface res;
    mat4 m1 = inverse(matrices_buffer.matrices[2]) * inverse(matrices_buffer.matrices[1]) * inverse(matrices_buffer.matrices[0]);
    mat4 m2 = mat4(1) * inverse(matrices_buffer.matrices[2]);// * inverse(matrices_buffer.matrices[3]);//  inverse(matrices_buffer.matrices[4]);
    res = unite(unite(SDF_box(pos, m1, box_buffer.boxes[0]), SDF_sphere(pos, mat4(1), sphere_buffer.spheres[0])), SDF_box(twist(pos, m2), mat4(1), box_buffer.boxes[1]));
    sphere_buffer.spheres[0].radius = 0.2;
    res = unite(SDF_box(twist(pos, m2), mat4(1), box_buffer.boxes[0]), SDF_sphere(pos - c_, mat4(1), sphere_buffer.spheres[0]));
    return res;
}
*/

Surface SDF_scene(vec3 p)
{
	vec4 pos = vec4(p.xyz, 1);	Surface res;
	res = unite(sub(sub(sub(sub(sub(sub(SDF_sphere(inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * pos, sphere_buffer.spheres[0]), SDF_box(inverse(matrices_buffer.matrices[2]) * bend(inverse(matrices_buffer.matrices[3]) * twist(inverse(matrices_buffer.matrices[4]) * inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * pos, inverse(matrices_buffer.matrices[4]) * inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * mat4(1), twist_buffer.twists[0]), mat4(1), bend_buffer.bends[0]), box_buffer.boxes[0])), SDF_box(inverse(matrices_buffer.matrices[2]) * bend(inverse(matrices_buffer.matrices[3]) * twist(inverse(matrices_buffer.matrices[5]) * inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * pos, inverse(matrices_buffer.matrices[5]) * inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * mat4(1), twist_buffer.twists[0]), mat4(1), bend_buffer.bends[0]), box_buffer.boxes[0])), SDF_box(inverse(matrices_buffer.matrices[2]) * bend(inverse(matrices_buffer.matrices[3]) * twist(inverse(matrices_buffer.matrices[6]) * inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * pos, inverse(matrices_buffer.matrices[6]) * inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * mat4(1), twist_buffer.twists[0]), mat4(1), bend_buffer.bends[0]), box_buffer.boxes[0])), SDF_box(inverse(matrices_buffer.matrices[2]) * bend(inverse(matrices_buffer.matrices[3]) * twist(inverse(matrices_buffer.matrices[7]) * inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * pos, inverse(matrices_buffer.matrices[7]) * inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * mat4(1), twist_buffer.twists[0]), mat4(1), bend_buffer.bends[0]), box_buffer.boxes[0])), SDF_box(inverse(matrices_buffer.matrices[2]) * bend(inverse(matrices_buffer.matrices[3]) * twist(inverse(matrices_buffer.matrices[8]) * inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * pos, inverse(matrices_buffer.matrices[8]) * inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * mat4(1), twist_buffer.twists[0]), mat4(1), bend_buffer.bends[0]), box_buffer.boxes[0])), SDF_box(inverse(matrices_buffer.matrices[2]) * bend(inverse(matrices_buffer.matrices[3]) * twist(inverse(matrices_buffer.matrices[9]) * inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * pos, inverse(matrices_buffer.matrices[9]) * inverse(matrices_buffer.matrices[10]) * inverse(matrices_buffer.matrices[1]) * mat4(1), twist_buffer.twists[0]), mat4(1), bend_buffer.bends[0]), box_buffer.boxes[0])), sub(unite(SDF_box(inverse(matrices_buffer.matrices[12]) * bend(inverse(matrices_buffer.matrices[13]) * inverse(matrices_buffer.matrices[15]) * pos, mat4(1), bend_buffer.bends[1]), box_buffer.boxes[2]), SDF_box(inverse(matrices_buffer.matrices[12]) * bend(inverse(matrices_buffer.matrices[13]) * inverse(matrices_buffer.matrices[14]) * inverse(matrices_buffer.matrices[15]) * pos, mat4(1), bend_buffer.bends[1]), box_buffer.boxes[2])), SDF_sphere(inverse(matrices_buffer.matrices[16]) * pos, sphere_buffer.spheres[1])));
	return res;
}

/*****
 * Utils
 *****/

vec3 get_norm(vec3 pos)
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
    norm.x += SDF_scene(pos + step.xyy).sdf - SDF_scene(pos - step.xyy).sdf;
    norm.y += SDF_scene(pos + step.yxy).sdf - SDF_scene(pos - step.yxy).sdf;
    norm.z += SDF_scene(pos + step.yyx).sdf - SDF_scene(pos - step.yyx).sdf;

    return normalize(norm);
}

vec3 lightResponse(vec3 vertexPosition, vec3 vertexNormal, vec3 vertexColor) {
    vec3 V = normalize(vertexPosition - cam_pos), L = normalize(lightDirection);
    vec3 normal = normalize(vertexNormal);
    vec3 RV = normalize(reflect(V, normal)), H = normalize(V - L);

    vec3 ambientOcculusion = vec3((1 - abs(dot(RV, normal))) / 5);
    float diffuse = pow((dot(normal, -L) + 1) / 1.7, 2) + 0.1;
    float pointColorWeight = 2.5, lightColorWeight = 1;
    float summaryWeight = pointColorWeight + lightColorWeight;
    vec3 pointRawColor = vertexColor * pointColorWeight;
    vec3 lightRawColor = lightColor * lightColorWeight;
    vec3 pointNaturalColor = vec3(max(min((pointRawColor.x + lightRawColor.x) / summaryWeight, 1.0), 0.0),
    max(min((pointRawColor.y + lightRawColor.y) / summaryWeight, 1.0), 0.0),
    max(min((pointRawColor.z + lightRawColor.z) / summaryWeight, 1.0), 0.0));
    return pointNaturalColor * diffuse - ambientOcculusion;
}

Material trace(vec3 org, vec3 dir)
{
    float t = 0;
    while (t < max_dist)
    {
        vec3 pos = org + dir * t;
        vec3 c = vec3(1, 1, 1);
        Surface srf = SDF_scene(pos);

        if (srf.sdf < eps)
        {
            //res = average_mtl(pos);
            srf.mtl.color = vec4(1, 0, 0, 1);
            //srf.mtl.color *= 0.4 * abs(dot(light_dir, get_norm(pos, -dir))) + 0.4;
            float depth = 1 / length(cam_pos - pos) * 50;
            vec3 nrm = get_norm(pos);
            depth = min(pow(depth, 1.2), 1.0);
            srf.mtl.color = vec4(lightResponse(pos, nrm, vec3(1, 0, 0)) * depth, 1);
            //srf.mtl.color = vec4(nrm.xyz, 1);
            //srf.mtl.color = vec4(get_norm(pos, -dir), 1);
            //srf.mtl.color = vec4(1, 0, 0, 1);
            //res.color = vec4(1, 0, 0, 1);
            return srf.mtl;
        }
        t += srf.sdf;
    }
    Material res;
    //res.color = vec4(vec3(176, 196, 222) / 255, 1);
    res.color = vec4(0, 0, 0, 1);
    return res;
}


// Main shader program function
void main() {
    //outColor = vec4(inColor + vec3(1, 1, 1), 1);
    /*
    vec3 cam_pos = normalize(vec3(abs(cos(time * 0 + 1)), 0.7, abs(sin(time * 0 + 1)))) * 5;
    vec3 cam_dir = normalize(vec3(0) - cam_pos);
    vec3 up = vec3(0, 1, 0);
    vec3 cam_right = normalize(cross(cam_dir, up));
    vec3 cam_up = normalize(cross(cam_right, cam_dir));
    */
    float near = 1;

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
    //outColor = vec4(matrices_buffer.matrices[1][j][i] / 2, 0, 0, 1);
    //outColor = vec4(sphere_buffer.spheres[1].radius, 0, 0, 1);
    //v = figure_buffer.matr * v;
    //outColor = sphere_buffer.spheres[0].mtl.color;
    //Material res = trace()''
    //outColor = vec4(1, 0, 0, 1);

} // End of 'main' function
