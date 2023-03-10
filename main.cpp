#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

void on_window_size(GLFWwindow* win, int width, int height) {
	std::cout << "win: " << width << "x" << height << std::endl;
	glViewport(0, 0, width, height);
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

	stbi_set_flip_vertically_on_load(true);

	float vertices[] = { // pos, col, texcoord
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};

	unsigned char indices[] = {
		0,1,3,
		1,2,3
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

	// load texture
	int width, height, channels;
	unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &channels, 0);
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	data = stbi_load("textures/awesomeface.png", &width, &height, &channels, 0);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *) (3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void *) (6*sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glViewport(0, 0, 800, 600);
	Shader shader("shader/tex1_vs.glsl", "shader/tex1_fs.glsl");
	shader.use();
	shader.setInt("Texture1", 0);
	shader.setInt("Texture2", 1);

	while (!glfwWindowShouldClose(window)) {
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
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
