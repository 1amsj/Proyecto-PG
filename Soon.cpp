#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Soon.h"
#include "stb_easy_font.h"
#include <cmath>
#include <cstring>
#include <iostream>


void framebuffer_size_callback1(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

}

// esc para cerrar la pantalla
void processInput1(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void drawSun1(float cx, float cy, float radius) {
    glColor3f(1.0f, 0.9f, 0.0f);  // Sol brillante
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * 3.1416f * i / 100;
        glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
    }
    glEnd();
}

void drawTextCentered1(GLFWwindow* window, const char* text, float scale, float offsetY = 0.0f) {
    char buffer[99999];
    int num_quads = stb_easy_font_print(0, 0, (char*)text, NULL, buffer, sizeof(buffer));

    int winWidth, winHeight;
    glfwGetFramebufferSize(window, &winWidth, &winHeight);

    float textPixelWidth = 9.0f * strlen(text) * scale;
    float x = (winWidth - textPixelWidth) / 2.0f + 190.0f;
    float y = winHeight / 2.0f + offsetY;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, winWidth, 0, winHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(x, y, 0.0f);
    glScalef(scale, -scale, 1.0f);

    glColor3f(1.0f, 1.0f, 1.0f);  // Texto blanco
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void drawCircle1(float x, float y, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 100; i++) {
        float angle = i * 2.0f * 3.1416f / 100;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

void drawCloud(float x, float y, float scale) {
    float radius = 0.1f * scale;
    drawCircle1(x, y + radius * 0.8f, radius * 0.8f, 1.0f, 1.0f, 1.0f);
    drawCircle1(x, y, radius, 1.0f, 1.0f, 1.0f);
    drawCircle1(x - radius * 0.9f, y + radius * 0.3f, radius * 0.8f, 1.0f, 1.0f, 1.0f);
    drawCircle1(x + radius * 0.9f, y + radius * 0.3f, radius * 0.8f, 1.0f, 1.0f, 1.0f);
    drawCircle1(x - radius * 0.5f, y - radius * 0.4f, radius * 0.7f, 1.0f, 1.0f, 1.0f);
    drawCircle1(x + radius * 0.5f, y - radius * 0.4f, radius * 0.7f, 1.0f, 1.0f, 1.0f);
}


void drawClouds() {
    drawCloud(-0.75f, 0.75f, 1.0f);  // Grande a la izquierda
    drawCloud(-0.2f, 0.85f, 0.6f);   // Pequeña arriba centro
    drawCloud(0.5f, 0.7f, 0.9f);     // Mediana derecha
    drawCloud(0.1f, 0.6f, 0.7f);     // Centro inferior
    drawCloud(-0.5f, 0.6f, 0.5f);    // Izquierda inferior
}

void Soon::drawWindow2() {
    GLFWwindow* window = glfwCreateWindow(1000, 800, "Proximamente", NULL, NULL);
    if (!window) {
        std::cout << "No se pudo crear la ventana." << std::endl;
        return;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback1);

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
        processInput1(window);
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);  // Fondo celeste
        glClear(GL_COLOR_BUFFER_BIT);

        // Configurar matriz ortográfica normal
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        drawSun1(0.75f, 0.75f, 0.15f);  // Sol
        drawClouds();

        // Texto centrado como en tu ejemplo
        drawTextCentered1(window, "Proximamente", 10.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwWaitEventsTimeout(1.0 / 60.0);
    }

    glfwDestroyWindow(window);
}
