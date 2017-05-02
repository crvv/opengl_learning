#version 400 core

uniform float terrainSideLength;

in vec4 position;
flat in int road;

out vec4 fragColor;

void main() {
    if (road == 1) {
        fragColor = vec4(0.8, 0.8, 0.8, 1);
        return;
    }
    if (road == 2) {
        fragColor = vec4(0, 1, 0, 1);
        return;
    }
    if (road == 3) {
        fragColor = vec4(1, 0, 0, 1);
        return;
    }
    if (road == 10) {
        fragColor = vec4(0.4, 0.4, 0.4, 1);
        return;
    }
    if (road == 8) {
        gl_FragColor = vec4(1, 1, 1, 1);
        return;
    }

	float height = position.z * terrainSideLength;
    float red = 0;
    float green = 0;
    float blue = 0;
    if (height < 1) {
        blue = 1;
    } else {
        green = 1 - height / 2000;
        red = 1 - green;
    }
	fragColor = vec4(red, green, blue, 1) * 0.6;
}
