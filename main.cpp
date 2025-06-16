#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Sky.h"
#include "Uso.h"
#include "Presentacion.h"

bool ventana2 = false, ventana3 = false;
// para cambiar el tamaño de la pantalla
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

}

// esc para cerrar la pantalla
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


	GLFWwindow* window = glfwCreateWindow(1000, 800, "Bienvenido", NULL, NULL);
	if (window == NULL) {
		std::cout << "Se falló en la creación de la ventana de GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Se falló en la creación de la ventana de GLAD" << std::endl;
		return -1;
	}
	Uso uso;
	Sky sky;
	presentacion presenta;

	glViewport(0, 0, 1000, 800);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, presentacion::mouse_callback);

	//centrar la pantalla

	GLFWmonitor* pantalla = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(pantalla);
	if (mode) {
		int sceenWidth = mode->width;
		int screenHeight = mode->height;
		int posX = (sceenWidth - 1000) / 2;
		int posY = (screenHeight - 800) / 2;
		glfwSetWindowPos(window, posX, posY);
	}

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.05f, 0.05f, 0.2f, 0.8f);
		glClear(GL_COLOR_BUFFER_BIT);

		presenta.drawButton();


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	if (ventana2)
		uso.drawWindow1();

	if (ventana3)
		sky.drawSky();

	glfwTerminate();
	return 0;
}