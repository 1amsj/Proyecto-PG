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

	float dayProgress = 0.0f;  // Rango: 0.0 a 24.0

    while (!glfwWindowShouldClose(window2)) {
    dayProgress += 0.001f;  // Más lento
    if (dayProgress >= 24.0f) {
        dayProgress = 0.0f;
    }

    float r = 0.0f, g = 0.0f, b = 0.0f;

    if (dayProgress < 6.0f) {
        // Amanecer (0 - 6)
        float t = dayProgress / 6.0f;
        r = 0.8f + 0.2f * t;  // de 0.8 a 1.0
        g = 0.4f + 0.3f * t;  // de 0.4 a 0.7
        b = 0.2f + 0.4f * t;  // de 0.2 a 0.6
    } else if (dayProgress < 12.0f) {
        // Día (6 - 12)
        r = 0.53f;
        g = 0.81f;
        b = 0.98f;
    } else if (dayProgress < 18.0f) {
        // Atardecer (12 - 18)
        float t = (dayProgress - 12.0f) / 6.0f;
        r = 1.0f;
        g = 0.8f - 0.5f * t;  // de 0.8 a 0.3
        b = 0.6f - 0.5f * t;  // de 0.6 a 0.1
    } else {
        // Noche (18 - 24)
        float t = (dayProgress - 18.0f) / 6.0f;
        r = 0.05f;
        g = 0.05f + 0.05f * (1.0f - t);  // mantiene un pequeño degradado
        b = 0.2f + 0.1f * (1.0f - t);
    }

    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window2);
    glfwPollEvents();
}
