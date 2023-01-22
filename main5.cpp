#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

using namespace std;

void on_window_size(GLFWwindow* win, int width, int height) {
	std::cout << "win: " << width << "x" << height << std::endl;
	glViewport(0, 0, width, height);
}

void check(unsigned int shader) {
	int success;
	char info[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader,512,NULL,info);
		cerr << "Error: " << info << endl;
	}
}

void checkLinking(unsigned int program) {
	int status;
	char info[512];
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		glGetProgramInfoLog(program,512,NULL,info);
		cerr << "ERROR: Link => " << info << endl;
	}
}

int main (int argc, char *argv[])
{
	if (glfwInit() == GLFW_FALSE) {
		cerr << "Error initializing GFLW3" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	std::cout << "Init..." << std::endl;

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	if (window == NULL) {
		cerr << "Error creating GLFW window" << endl;
		glfwTerminate();
		return -2;
	}
	glfwMakeContextCurrent(window);
	std::cout << "Window..." << std::endl;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cerr << "error loading glad load" << endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	glfwSetFramebufferSizeCallback(window, on_window_size);

	int nrattrib;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrattrib);
	std::cout << "max vertex attribs: " << nrattrib << std::endl;

	float vertices[] = {
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	unsigned char indices[] = {
		0,1,2,
	};

	unsigned int vbo, vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *) (3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	bool wireframe_mode = false;
	glViewport(0, 0, 800, 600);
	Shader shader("shader/simple_vertex.glsl", "shader/simple_fragment.glsl");

	while (!glfwWindowShouldClose(window)) {
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			wireframe_mode = !wireframe_mode;
		if (wireframe_mode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		int ucol = glGetUniformLocation(shader.ID, "color");
		shader.use();
		shader.setFloat("offset", 0.5f);
		float green = 0.5f * cosf(glfwGetTime()) + 0.5f;
		glUniform4f(ucol, 1.0f, green, 0.0f, 1.0f);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
end:
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
