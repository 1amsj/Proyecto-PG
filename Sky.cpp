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
        time += 0.01f;  // Simula el paso del tiempo
        float r = 0.0f, g = 0.0f, b = 0.0f;

        // Cálculo del progreso del día en un rango de 0 a 24
        float dayProgress = fmod(time, 24.0f);

        if (dayProgress < 6.0f) {
            // Amanecer (0 - 6)
            r = 1.0f;
            g = 0.5f + 0.05f * dayProgress;
            b = 0.2f + 0.1f * dayProgress;
        } else if (dayProgress < 12.0f) {
            // Día (6 - 12)
            r = 0.53f;
            g = 0.81f;
            b = 0.98f;
        } else if (dayProgress < 18.0f) {
            // Atardecer (12 - 18)
            r = 1.0f;
            g = 0.4f - 0.04f * (dayProgress - 12.0f);
            b = 0.2f - 0.03f * (dayProgress - 12.0f);
        } else {
            // Noche (18 - 24)
            r = 0.05f;
            g = 0.05f;
            b = 0.2f;
        }

	glfwDestroyWindow(window2);

}
