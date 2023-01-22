#version 330 core
in vec2 outTexCoord;
in vec3 outCol;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
void main() {
	//gl_FragColor = texture(Texture2, outTexCoord)*vec4(outCol, 1.0);
	gl_FragColor = mix(texture(Texture1, outTexCoord), texture(Texture2, outTexCoord), 0.2f);
}
