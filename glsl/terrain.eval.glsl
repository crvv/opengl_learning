#version 400 core

layout (quads, equal_spacing, ccw) in;

uniform sampler2D terrain;
uniform mat4 mvp;

uniform int currentPoint;
uniform int len;
uniform float xs[8], ys[8], as[8], bs[8], cs[8];

out vec4 position;
flat out int pointType;

// road
float halfWidth = 0.0015;
float height = 0.001;

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

float calculateRoadz(float x, float y, float roady, float dist, vec2 normalVector) {
    vec2 roadxy;
    if (y >= roady) {
        roadxy = vec2(x, y) - normalVector * dist;
    } else {
        roadxy = vec2(x, y) + normalVector * dist;
    }
    float roadz = 0;
    int neighbors = 20;
    for (int i = -neighbors; i <= neighbors; i++) {
        roadz += texture(terrain, roadxy + i * normalVector * halfWidth / 2).r;
    }
    roadz /= (2 * neighbors + 1);
    return roadz;
}

bool inArea(float x, float y) {
    if (x < 0.234) {
        return false;
    }
    if (y < 0.334) {
        return false;
    }
    if (y > x + 0.3) {
        return false;
    }
    if (y > -x + 1.2) {
        return false;
    }
    if (y < 0.8 * x - 0.1) {
        return false;
    }
    return true;
}

void main() {
    pointType = 0;
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    float x = mix(gl_in[0].gl_Position.x, gl_in[1].gl_Position.x, u);
    float y = mix(gl_in[0].gl_Position.y, gl_in[2].gl_Position.y, v);
    float z = texture(terrain, vec2(x, y)).r;

    float roady = spline(x);
    float roadd = deriv(x);
    vec2 normalVector = normalize(vec2(-roadd, 1));
    float dist = abs(dot(vec2(0, y - roady), normalVector));

    if (dist < halfWidth * 1.5) {
        float roadz = calculateRoadz(x, y, roady, dist, normalVector);
        pointType = 1;
        if (dist < halfWidth * 0.8) {
            z = roadz + height;
        } else if (dist < halfWidth) {
            z = roadz + height * 1.2;
        } else {
            pointType = 10;
            z += (3 - 2 * dist / halfWidth) * (roadz + height * 1.2 - z);
        }
    }

    for (int i = 0; i < len; i++) {
        if (length(vec2(x - xs[i], y - ys[i])) < 0.002) {
            pointType = 2;
            if (i == currentPoint) {
                pointType = 3;
            }
            break;
        }
    }

    if (inArea(2 * x - 0.2, 2 * y - 0.1)) {
        z = 0;
        pointType = 8;
    } else if (dist > 0.05 && (int(x * 10000) % 500 < 50) && (int(y * 10000) % 500 < 50)) {
        pointType = 9;
    }

    position = vec4(x, y, z, 1.0);
    gl_Position = mvp * position;
}
