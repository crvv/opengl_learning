#version 400 core

layout (quads, equal_spacing, ccw) in;

uniform sampler2D terrain;
uniform mat4 mvp;

uniform int len;
uniform float xs[64], ys[64], as[64], bs[64], cs[64];

out vec4 position;
flat out int road;

int lower_bound(float x) {
    int idx;
    for (idx = 0; idx < len; idx++) {
        if (x < xs[idx]) {
            break;
        }
    }
    idx--;
    return max(0, idx);
}
float spline(float x) {
    int idx = lower_bound(x);
    float dx = x - xs[idx];
    if (x < xs[0]) {
        return (bs[0] * dx + cs[0]) * dx + ys[0];
    }
    if (x > xs[len - 1]) {
        return (bs[len - 1] * dx + cs[len - 1]) * dx + ys[len - 1];
    }
    return ((as[idx] * dx + bs[idx]) * dx + cs[idx]) * dx + ys[idx];
}

float deriv(float x) {
    int idx = lower_bound(x);
    float dx = x - xs[idx];
    if (x < xs[0]) {
        return 2.0 * bs[0] * dx + cs[0];
    }
    if (x > xs[len - 1]) {
        return 2.0 * bs[len - 1] * dx + cs[len - 1];
    }
    return (3.0 * as[idx] * dx + 2.0 * bs[idx]) * dx + cs[idx];

}

void main() {
    road = 0;
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    float x = mix(gl_in[0].gl_Position.x, gl_in[1].gl_Position.x, u);
    float y = mix(gl_in[0].gl_Position.y, gl_in[2].gl_Position.y, v);
    float z = texture(terrain, vec2(x, y)).r;

    float roady = spline(x);
    float roadd = deriv(x);
    vec2 normalVector = normalize(vec2(-roadd, 1));
    float dist = abs(dot(vec2(0, y - roady), normalVector));

    vec2 roadxy = vec2(x, y) - normalVector * dist;
    float roadz = texture(terrain, roadxy).r;

    float halfWidth = 0.0040;
    float height = 0.0005;
    if (dist < halfWidth) {
        road = 1;
        if (dist < halfWidth * 0.6) {
            z = roadz + height;
        } else if (dist < halfWidth * 0.8) {
            z = roadz + height * 1.2;
        } else {
            z = roadz + (halfWidth - dist);
        }
    }

    for (int i = 0; i < len; i++) {
        if (length(vec2(x - xs[i], y - ys[i])) < 0.0025) {
            road = 2;
            break;
        }
    }
    position = vec4(x, y, z, 1.0);
    gl_Position = mvp * position;
}
