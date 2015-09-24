#version 400 core

uniform float terrainSideLength;

in vec4 position;

void main() {
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