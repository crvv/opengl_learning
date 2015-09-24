#version 400 core

layout (quads, equal_spacing, ccw) in;

uniform sampler2D terrain;
uniform mat4 mvp;

out vec4 position;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    float a = mix(gl_in[0].gl_Position.x, gl_in[1].gl_Position.x, u);
    float b = mix(gl_in[0].gl_Position.y, gl_in[2].gl_Position.y, v);
    vec2 xy = vec2(a, b);
    float z = texture(terrain, xy).r;
    position = vec4(xy, z, 1.0);
    gl_Position = mvp * position;
}
