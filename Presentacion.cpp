#include <glad/glad.h>
#include "Presentacion.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <cmath>

// variables para los botones
const float buttonX = -0.2f;
const float buttonY = -0.2f;
const float buttonWidth = 0.4f;
const float buttonHeight = 0.2f;

//variables para las estrellas
const int points = 5;
const float vertices = 2 * 3.1416 / (points * 2);

// variables para la nueva ventana
extern bool ventana2;
bool buttonClicked = false;

// dibujamos la estrella
void drawStar(float centrox, float centroy, float radio, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centrox, centroy);

    for (int i = 0; i <= points * 2; i++) {
        float angulo = i * vertices;
        float rad = (i % 2 == 0) ? radio : radio * 0.5f;
        float x = centrox + cos(angulo) * rad;
        float y = centroy + sin(angulo) * rad;
        glVertex2f(x, y);
    }
    glEnd();

}

//generamos las estrellas
void presentacion::drawStars() {
    drawStar(-0.8f, 0.9f, 0.02f, 1.0f, 1.0f, 0.8f);
    drawStar(-0.6f, 0.7f, 0.03f, 1.0f, 1.0f, 0.8f);
    drawStar(-0.3f, 0.8f, 0.025f, 1.0f, 1.0f, 0.8f);
    drawStar(0.0f, 0.95f, 0.03f, 1.0f, 1.0f, 0.8f);
    drawStar(0.3f, 0.85f, 0.02f, 1.0f, 1.0f, 0.8f);
    drawStar(0.6f, 0.75f, 0.035f, 1.0f, 1.0f, 0.8f);
    drawStar(0.8f, 0.9f, 0.025f, 1.0f, 1.0f, 0.8f);
    drawStar(-0.9f, 0.6f, 0.015f, 1.0f, 1.0f, 0.8f);
    drawStar(0.7f, 0.6f, 0.02f, 1.0f, 1.0f, 0.8f);
    drawStar(-0.1f, 0.7f, 0.018f, 1.0f, 1.0f, 0.8f);
    drawStar(-0.85f, -0.2f, 0.02f, 1.0f, 1.0f, 0.8f);
    drawStar(-0.65f, -0.5f, 0.018f, 1.0f, 1.0f, 0.8f);
    drawStar(-0.45f, -0.3f, 0.015f, 1.0f, 1.0f, 0.8f);
    drawStar(-0.2f, -0.6f, 0.02f, 1.0f, 1.0f, 0.8f);
    drawStar(0.0f, -0.4f, 0.025f, 1.0f, 1.0f, 0.8f);
    drawStar(0.25f, -0.7f, 0.017f, 1.0f, 1.0f, 0.8f);
    drawStar(0.5f, -0.3f, 0.02f, 1.0f, 1.0f, 0.8f);
    drawStar(0.7f, -0.5f, 0.022f, 1.0f, 1.0f, 0.8f);
    drawStar(0.9f, -0.8f, 0.015f, 1.0f, 1.0f, 0.8f);
    drawStar(-0.3f, -0.85f, 0.02f, 1.0f, 1.0f, 0.8f);


}

void drawMoon(float cx, float cy, float radius, float r, float g, float b) {
    const int segments = 200;

    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // Centro

    for (int i = 0; i <= segments; i++) {
        float angulo = 2.0f * 3.1415926f * i / segments;
        float x = radius * cosf(angulo);
        float y = radius * sinf(angulo);
        glVertex2f(cx + x, cy + y);
    }

    glEnd();
}



void drawCircle(float cx, float cy, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // centro
    for (int i = 0; i <= segments; ++i) {
        float angle = i * 2.0f * 3.1416 / segments;
        float x = cx + cos(angle) * radius;
        float y = cy + sin(angle) * radius;
        glVertex2f(x, y);
    }
    glEnd();
}

void presentacion::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Convertir a coordenadas NDC (-1 a 1)
        float x = (xpos / width) * 2.0f - 1.0f;
        float y = 1.0f - (ypos / height) * 2.0f;

        // Verificar si el clic está dentro del botón
        if (x >= buttonX && x <= buttonX + buttonWidth &&
            y >= buttonY && y <= buttonY + buttonHeight) {
            buttonClicked = !buttonClicked;  // Cambia color al hacer clic
            std::cout << "Bienvenido" << std::endl;
            ventana2 = true;  // Cambia a la segunda ventana
            glfwSetWindowShouldClose(window, true);
        }


    }

}
void presentacion::drawButton() {
    float radius = 0.05f;
    int segments = 20;

    float left = buttonX + radius;
    float right = buttonX + buttonWidth - radius;
    float bottom = buttonY + radius;
    float top = buttonY + buttonHeight - radius;

    drawStars();
    drawMoon(0.75f, 0.75f, 0.1f, 1.0f, 1.0f, 0.85f);


    if (buttonClicked)
        glColor4f(0.9f, 0.9f, 0.85f, 0.8f);
    else
        glColor4f(1.0f, 1.0f, 0.0f, 0.8f);

    glBegin(GL_QUADS);
    glVertex2f(left, buttonY);
    glVertex2f(right, buttonY);
    glVertex2f(right, buttonY + buttonHeight);
    glVertex2f(left, buttonY + buttonHeight);

    glVertex2f(buttonX, bottom);
    glVertex2f(buttonX + buttonWidth, bottom);
    glVertex2f(buttonX + buttonWidth, top);
    glVertex2f(buttonX, top);
    glEnd();

    // Esquinas del botón redondeadas
    drawCircle(left, top, radius, segments);
    drawCircle(right, top, radius, segments);
    drawCircle(left, bottom, radius, segments);
    drawCircle(right, bottom, radius, segments);
}