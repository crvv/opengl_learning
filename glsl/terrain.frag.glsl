#version 400 core

uniform float terrainSideLength;

in vec4 position;
flat in int road;

void main() {
    if (road == 1) {
        gl_FragColor = vec4(0.5, 0.5, 0.5, 1);
        return;
    }
    if (road == 2) {
        gl_FragColor = vec4(0, 1, 0, 1);
        return;
    }
    if (road == 3) {
        gl_FragColor = vec4(1, 0, 0, 1);
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
	gl_FragColor = vec4(red, green, blue, 1) * 0.6;
}