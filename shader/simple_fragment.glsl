#version 330 core
in vec3 outPos;
in vec3 outCol;
out vec4 FragColor;
uniform vec4 color;
void main() {
	FragColor = vec4(outCol, 1.0f)*color;
}
