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
uniform int is_bulb;
uniform vec3 bulb_pos;
uniform vec3 bulb_color;

/*****
 * Globals
 *****/

float max_dist = 20; // max ray traversal distance
float eps = 0.001;
vec3 light_dir = normalize(vec3(1, 3, 3));
vec3 lightColor = vec3(0.7);


/*****
 * Structures
 *****/

struct Material
{
    vec4 color;
    int is_light_source;
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

Surface smin(Surface a, Surface b)
{
    //float k = 0.1; // for ex 5
    //float k = 0.8; // for ex 1
    float k = 0.7; // for ex 6
    float h = clamp( 0.5+0.5*(b.sdf-a.sdf)/k, 0.0, 1.0 );
    Surface res;
    res.sdf = mix( b.sdf, a.sdf, h ) - k*h*(1.0-h);
    res.mtl.color = mix(b.mtl.color, a.mtl.color, h);
    res.mtl.is_light_source = 0;
    return res;
}

Surface sunite(Surface a, Surface b)
{
    Surface res;
    res = smin(a, b);
    return res;
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
    Surface res;
    if (a.sdf < b.sdf) {
        res.sdf = b.sdf;
        res.mtl = b.mtl;
    } else {
        res.sdf = a.sdf;
        res.mtl = a.mtl;
    }

    if (a.mtl.color.xyz == vec3(0)) {
        res.mtl = b.mtl;
    }
    else if (b.mtl.color.xyz == vec3(0)){
        res.mtl = a.mtl;
    }
    return res;
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
    res.mtl = sphere.mtl;
    return res;
}




Surface SDF_box(vec4 p, Box box)
{
    vec3 pos = p.xyz;
    Surface res;
    vec3 q = abs(pos) - vec3(box.radius / 2);

    res.sdf = length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
    res.mtl = box.mtl;
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

#include SDF_scene

/*****
 * Utils
 *****/

vec3 get_norm(vec3 pos)
{
    vec2 step = vec2(eps, 0);
    vec3 norm;
    norm.x += SDF_scene(pos + step.xyy).sdf - SDF_scene(pos - step.xyy).sdf;
    norm.y += SDF_scene(pos + step.yxy).sdf - SDF_scene(pos - step.yxy).sdf;
    norm.z += SDF_scene(pos + step.yyx).sdf - SDF_scene(pos - step.yyx).sdf;

    return normalize(norm);
}

float softshadow( in vec3 ro, in vec3 rd, float mint, float maxt, float k )
{
    float res = 1.0;
    float t = mint;
    for( int i=0; i < 256 && t<maxt; i++ )
    {
        float h = SDF_scene(ro + rd*t).sdf;
        if( h<0.001 )
        return 0.0;
        res = min( res, k*h/t );
        t += h;
    }
    return res;
}

float bulblight( in vec3 ro, float mint, float maxt, float k )
{
    vec3 rd = normalize(bulb_pos - ro);
    float res = 1.0;
    float t = mint;
    for( int i=0; i < 256 && t<maxt; i++ )
    {
        Surface srf = SDF_scene(ro + rd*t);
        if (srf.sdf < 0.001) {
            if (srf.mtl.is_light_source == 1) {
                return exp(1 - t);
            }
            return 0;
        }
        t += srf.sdf;
    }
    return 0;
}
Material reflection(vec3 org, vec3 dir) {
    float t = 0;
    while (t < max_dist)
    {
        vec3 pos = org + dir * t;
        vec3 c = vec3(1, 1, 1);
        Surface srf = SDF_scene(pos);

        if (srf.sdf < eps)
        {
            return srf.mtl;
        }
        t += srf.sdf;
    }
    Material res;
    res.color = vec4(0, 0, 0, 1);
    return res;
}

float AmbientOc( vec3 Org, vec3 Dir, float tmax, float s )
{
    float t = 0, d = 0, oc = 0, dt = .4;

    while (t < tmax)
    {
        d = SDF_scene(Org + Dir * t * dt).sdf;
        oc += (1 / pow(2, t)) * (t * dt - d);
        t++;
    }
    return 1 - clamp(s * oc, 0, 1);
}

vec3 lightResponse(vec3 vertexPosition, vec3 vertexNormal, vec3 vertexColor) {
    vec3 lightDirection = -light_dir;
    vec3 cameraPosition = cam_pos;
    vec3 V = normalize(vertexPosition - cameraPosition), L = normalize(lightDirection);
    vec3 normal = normalize(vertexNormal);
    vec3 RV = normalize(reflect(V, normal)), H = normalize(V - L);

    vec3 ambientOcculusion = vec3((1 - abs(dot(RV, normal))) / 5);
    float diffuse = max(pow((dot(normal, -L) + 1) / 1.7, 2), 0.08);
    float pointColorWeight = 5, lightColorWeight = 0.1;
    float summaryWeight = pointColorWeight + lightColorWeight;
    vec3 pointRawColor = vertexColor * pointColorWeight;
    vec3 lightRawColor = lightColor * lightColorWeight;
    vec3 pointNaturalColor = vec3(max(min((pointRawColor.x + lightRawColor.x) / summaryWeight, 1.0), 0.0),
    max(min((pointRawColor.y + lightRawColor.y) / summaryWeight, 1.0), 0.0),
    max(min((pointRawColor.z + lightRawColor.z) / summaryWeight, 1.0), 0.0));
    return pointNaturalColor * diffuse;
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
            //srf.mtl.color = vec4(1, 0, 0, 1);
            //srf.mtl.color *= 0.4 * abs(dot(light_dir, get_norm(pos, -dir))) + 0.4;
            vec3 nrm = get_norm(pos);
            //srf.mtl.color = vec4(lightResponse(pos, nrm, vec3(1, 0, 0)) * depth, 1);
            //srf.mtl.color = vec4(nrm.xyz, 1);
            //srf.mtl.color = vec4(get_norm(pos, -dir), 1);
            //srf.mtl.color = vec4(1, 0, 0, 1);
            //res.color = vec4(1, 0, 0, 1);
            //srf.mtl.color += rf.color * 0.1;
            //srf.mtl.color *= min(max(shd, 0.6), 1);
            //srf.mtl.color = vec4(nrm, 1);
            //srf.mtl.color = vec4(lightResponse(pos, nrm, srf.mtl.color.xyz) * depth, 1);
            if (srf.mtl.is_light_source == 0) {
                float shd = softshadow(pos + nrm * 0.2, light_dir, 0.1, 10, 10);
                srf.mtl.color = vec4(lightResponse(pos, nrm, srf.mtl.color.xyz), 1);
                srf.mtl.color *= min(max(AmbientOc(pos, nrm, 5, 0.3), 0.7), 1);
                srf.mtl.color *= min(max(shd, 0.7), 1);

                Material rf = reflection(pos + nrm * 0.1, reflect(dir, nrm));
                srf.mtl.color = srf.mtl.color * 0.9 + rf.color * 0.1;
                srf.mtl.color /= max(pow(t, 1.6), 25) / 20;
                if (is_bulb == 1) {
                    float lgh = bulblight(pos + nrm * 0.1, 0, 10, 20);
                    srf.mtl.color += vec4(bulb_color, 1) * lgh * 0.3;
                }
                //srf.mtl.color = vec4(nrm, 1);
            }


            return srf.mtl;
        }
        t += srf.sdf;
    }
    Material res;
    res.color = vec4(vec3(0) / 255, 1);
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
    float far = 2000;

    vec3 pixel_pos = (cam_dir * near +
    normalize(cam_up) * ((float(gl_FragCoord.y) / frame_h) - 0.5) +
    normalize(cam_right) * ((float(gl_FragCoord.x) / frame_w) - 0.5));

    vec3 pixel_pos2 = pixel_pos * (far / near);

    vec3 dir = normalize(pixel_pos);
    vec3 org = pixel_pos + cam_pos;

    float t = time * 2;
    //light_dir = normalize(2 * vec3(0, 1, 0) + vec3(sin(t), 0, cos(t)));
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
