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

	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};
	float vertices1[] = {
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};

	unsigned int vao[2], vbo[2];
	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);

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

	const char* fs_source2 = R"(
	#version 330 core
	out vec4 color;
	void main() {
		color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	}
	)";

	glShaderSource(fs, 1, &fs_source, 0);
	glCompileShader(fs);
	check_shader(fs);

	unsigned int fs1;
	fs1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs1, 1, &fs_source2, 0);
	glCompileShader(fs1);
	check_shader(fs1);

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	check_link(program);

	unsigned int program1;
	program1 = glCreateProgram();
	glAttachShader(program1, vs);
	glAttachShader(program1, fs1);
	glLinkProgram(program1);
	check_link(program1);



//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while(!glfwWindowShouldClose(window)) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(vao[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(program1);
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
