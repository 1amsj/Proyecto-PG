#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Sky.h"
#include <iostream>
#include <cmath>


void drawCircleM(float x, float y, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 100; i++) {
        float angle = i * 2.0f * 3.1416f / 100;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

void drawStar1() {
    glPointSize(2.5f);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 100; ++i) {
        float x = ((rand() % 200) - 100) / 100.0f;
        float y = ((rand() % 200) - 100) / 100.0f;
        glVertex2f(x, y);
    }
    glEnd();
}

void Sky::drawSky() {
    GLFWwindow* window2 = glfwCreateWindow(1000, 800, "Ciclo Solar", NULL, NULL);
    if (!window2) {
        std::cout << "Se falló en la creación de la ventana" << std::endl;
        return;
    }

    glfwMakeContextCurrent(window2);
    gladLoadGL();

    GLFWmonitor* pantalla = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(pantalla);
    if (mode) {
        int screenWidth = mode->width;
        int screenHeight = mode->height;
        int posX = (screenWidth - 1000) / 2;
        int posY = (screenHeight - 800) / 2;
        glfwSetWindowPos(window2, posX, posY);
    }

    float dayProgress = 0.0f;
    float speed = 0.02f;  // Velocidad más rápida

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window2)) {
        dayProgress += speed;
        if (dayProgress >= 24.0f) dayProgress = 0.0f;

        float r = 0.0f, g = 0.0f, b = 0.0f;
        if (dayProgress < 6.0f) {
            float t = dayProgress / 6.0f;
            r = 0.8f + 0.2f * t;
            g = 0.4f + 0.3f * t;
            b = 0.2f + 0.4f * t;
        }
        else if (dayProgress < 12.0f) {
            r = 0.53f;
            g = 0.81f;
            b = 0.98f;
        }
        else if (dayProgress < 18.0f) {
            float t = (dayProgress - 12.0f) / 6.0f;
            r = 1.0f;
            g = 0.8f - 0.5f * t;
            b = 0.6f - 0.5f * t;
        }
        else {
            float t = (dayProgress - 18.0f) / 6.0f;
            r = 0.05f;
            g = 0.05f + 0.05f * (1.0f - t);
            b = 0.2f + 0.1f * (1.0f - t);
        }

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);

        if (dayProgress < 12.0f) {
            float x = -0.9f + (dayProgress / 12.0f) * 1.8f;
            float y = 0.7f * sinf((dayProgress / 12.0f) * 3.1416f);
            drawCircleM(x, y, 0.1f, 1.0f, 1.0f, 0.0f);  // Sol
        }

        if (dayProgress >= 12.0f) {
            float x = -0.9f + ((dayProgress - 12.0f) / 12.0f) * 1.8f;
            float y = 0.7f * sinf(((dayProgress - 12.0f) / 12.0f) * 3.1416f);
            drawCircleM(x, y, 0.08f, 0.9f, 0.9f, 1.0f);  // Luna
        }

        // Estrellas solo de noche (18 a 24)
        if (dayProgress >= 18.0f && dayProgress < 24.0f) {
            drawStar1();
        }

        glfwSwapBuffers(window2);
        glfwPollEvents();
        glfwWaitEventsTimeout(1.0 / 60.0);
    }

    glfwDestroyWindow(window2);
}