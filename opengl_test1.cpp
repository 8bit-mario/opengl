#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void check_shader(unsigned int id) {
	int check;
	char info[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &check);
	if (!check) {
		glGetShaderInfoLog(id, 512, NULL, info);
		std::cerr << "Error: " << info << std::endl;
	}
}
void check_link(unsigned int id) {
	int check;
	char info[512];
	glGetProgramiv(id, GL_LINK_STATUS, &check);
	if(!check) {
		glGetProgramInfoLog(id, 512, NULL, info);
		std::cerr << "Error: " << info << std::endl;
	}
}
int main (int argc, char *argv[])
{
	if (glfwInit() == GLFW_FALSE) {
		std::cerr << "Error glfw init" << std::endl;
		return 0;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "Window", NULL, NULL);
	if (window == nullptr) {
		std::cerr << "Error window create" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Error loading glad" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}
	glViewport(0, 0, 800, 600);
	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	unsigned int program, vs, fs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	const char* vs_source = R"(
	#version 330 core
	layout(location=0) in vec3 aPos;
	void main() {
		gl_Position = vec4(aPos, 1.0f);
	}
	)";
	glShaderSource(vs, 1, &vs_source, 0);
	glCompileShader(vs);
	check_shader(vs);

	const char* fs_source = R"(
	#version 330 core
	out vec4 color;
	void main() {
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	)";

	glShaderSource(fs, 1, &fs_source, 0);
	glCompileShader(fs);
	check_shader(fs);

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	check_link(program);


	while(!glfwWindowShouldClose(window)) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao);
		glUseProgram(program);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
