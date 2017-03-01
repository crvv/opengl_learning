#version 400 core

layout (vertices = 4) out;
uniform sampler2D terrain;

uniform mat4 mvp;
uniform bool frameEnable;

uniform vec2 screenSize;

bool offscreen(vec4 vertex){
//    if(vertex.z < -3.0){
//        return true;
//    }
    if(any(lessThan(vertex.xy, vec2(-3.0)))) {
        return true;
    }
    if(any(greaterThan(vertex.xy, vec2(3.0)))) {
        return true;
    }
    return false;
}

vec4 project(vec4 vertex){
    vec4 result = mvp * vertex;
    result /= result.w;
    return result;
}

void main() {
    if (gl_InvocationID != 0) {
        gl_out[gl_InvocationID].gl_Position	= gl_in[gl_InvocationID].gl_Position;
        return;
    }

    vec2 xy0 = gl_in[0].gl_Position.xy;
    vec2 xy1 = gl_in[1].gl_Position.xy;
    vec2 xy2 = gl_in[2].gl_Position.xy;
    vec2 xy3 = gl_in[3].gl_Position.xy;

    float z0 = texture(terrain, xy0).r;
    float z1 = texture(terrain, xy1).r;
    float z2 = texture(terrain, xy2).r;
    float z3 = texture(terrain, xy3).r;

    vec4 v0 = project(vec4(xy0, z0, 1));
    vec4 v1 = project(vec4(xy1, z1, 1));
    vec4 v2 = project(vec4(xy2, z2, 1));
    vec4 v3 = project(vec4(xy3, z3, 1));

//    vec4 v0 = project(gl_in[0].gl_Position);
//    vec4 v1 = project(gl_in[1].gl_Position);
//    vec4 v2 = project(gl_in[2].gl_Position);
//    vec4 v3 = project(gl_in[3].gl_Position);

    float pixel = 8;
//    if (frameEnable) {
//        pixel *= 2;
//    }
    float level = 0;
    if(all(bvec4(offscreen(v0), offscreen(v1), offscreen(v2), offscreen(v3)))){
    } else {
        float d0 = max(distance(v0.xyz, v1.xyz), distance(v0.xyz, v3.xyz));
        float d1 = max(distance(v0.xyz, v2.xyz), distance(v1.xyz, v3.xyz));
        float d = max(d0, d1);
        level = d * length(screenSize) / pixel;
    }

    level = clamp(level, 1, 64);
    gl_TessLevelInner[0] = level;
    gl_TessLevelInner[1] = level;
    gl_TessLevelOuter[0] = level;
    gl_TessLevelOuter[1] = level;
    gl_TessLevelOuter[2] = level;
    gl_TessLevelOuter[3] = level;

    gl_out[gl_InvocationID].gl_Position	= gl_in[gl_InvocationID].gl_Position;
}
