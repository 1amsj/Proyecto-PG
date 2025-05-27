#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Sky.h"
#include <iostream>

//función donde se busca cómo crear otra pantalla y crear la animación del ciclo solar reflejada en el cielo
void Sky::drawSky() {
	GLFWwindow* window2 = glfwCreateWindow(1000, 800, "Ciclo Solar", NULL, NULL);
	if (!window2)
	{
		std::cout << "Se falló en la creación de la ventana" << std::endl;
		return;
	}

	glfwMakeContextCurrent(window2);
	gladLoadGL();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Se falló en la carga de GLAD" << std::endl;
		return;
	}

	GLFWmonitor* pantalla = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(pantalla);
	if (mode) {
		int sceenWidth = mode->width;
		int screenHeight = mode->height;
		int posX = (sceenWidth - 1000) / 2;
		int posY = (screenHeight - 800) / 2;
		glfwSetWindowPos(window2, posX, posY);
	}

	while (!glfwWindowShouldClose(window2)) {
		glClearColor(0.53f, 0.81f, 0.98f, 1.0f); // color celeste
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window2);
		glfwPollEvents();
	}

	glfwDestroyWindow(window2);

}