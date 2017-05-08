#version 400 core

uniform float terrainSideLength;

in vec4 position;
flat in int pointType;

out vec4 fragColor;

void main() {
    if (pointType == 1) {
        fragColor = vec4(0.8, 0.8, 0.8, 1);
        return;
    }
    if (pointType == 2) {
        fragColor = vec4(0, 1, 0, 1);
        return;
    }
    if (pointType == 3) {
        fragColor = vec4(1, 0, 0, 1);
        return;
    }
    if (pointType == 10) {
        fragColor = vec4(0.4, 0.4, 0.4, 1);
        return;
    }
    if (pointType == 8) {
        fragColor = vec4(0.1137, 0.5294, 0.5294, 1);
        return;
    }
    if (pointType == 9) {
        fragColor = vec4(0.8, 0.4941, 0.03922, 1);
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
