#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Uso.h"
#include <iostream>
#include <cmath>
#include "stb_easy_font.h"
#include <vector>
#include <string>
#include <sstream>

float boxX = -0.9f, boxY = 0.8f, boxWidth = 1.8f, boxHeight = 1.5f, escalaX = 0.008f, escalaY = 0.012f, margenX = 0.03f, margenY = 0.05f;
float maxWidth = (boxWidth - 2 * margenX);
char buffer[10000];

std::vector<std::string> lineSeparator(const char* texto, float maxWidth, float escalaX) {
    std::vector<std::string> lineas;
    std::istringstream stream(texto);
    std::string palabra, linea;

    while (stream >> palabra) {
        std::string temp = linea.empty() ? palabra : linea + " " + palabra;

        float ancho = temp.size() * 4.0f * escalaX;

        if (ancho > maxWidth) {
            if (!linea.empty()) {
                lineas.push_back(linea);
                linea = palabra;
            }
            else {
                lineas.push_back(palabra);
                linea.clear();
            }
        }
        else {
            linea = temp;
        }
    }

    if (!linea.empty()) {
        lineas.push_back(linea);
    }

    return lineas;
}


void drawCircle(float x, float y, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 100; i++) {
        float angle = i * 2.0f * 3.1416f / 100;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

void drawStars() {
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

void drawTransparentBoxWithBorder(float x, float y, float width, float height, float r, float g, float b, float alpha) {
    // Fondo transparente
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, alpha);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y - height);
    glVertex2f(x, y - height);
    glEnd();

    // Borde blanco visible
    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y - height);
    glVertex2f(x, y - height);
    glEnd();

    glDisable(GL_BLEND);
}


void Uso::drawWindow1() {
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
            drawCircle(x, y, 0.1f, 1.0f, 1.0f, 0.0f);  // Sol
        }

        if (dayProgress >= 12.0f) {
            float x = -0.9f + ((dayProgress - 12.0f) / 12.0f) * 1.8f;
            float y = 0.7f * sinf(((dayProgress - 12.0f) / 12.0f) * 3.1416f);
            drawCircle(x, y, 0.08f, 0.9f, 0.9f, 1.0f);  // Luna
        }

        // Estrellas solo de noche (18 a 24)
        if (dayProgress >= 18.0f && dayProgress < 24.0f) {
            drawStars();
        }


        drawTransparentBoxWithBorder(boxX, boxY, boxWidth, boxHeight, 0.1f, 0.1f, 0.1f, 0.6f);

        const char* texto = "Hola "
            "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut "
            "ah?";


        // Obtener líneas ajustadas
        std::vector<std::string> lineas = lineSeparator(texto, maxWidth, escalaX);

        // Dibujar líneas
        glPushMatrix();
        glTranslatef(boxX + 0.03f, boxY - 0.15f, 0.0f);
        glScalef(0.006f, -0.009f, 1.0f);


        float offsetY = 0.0f;
        for (int i = 0; i < lineas.size(); ++i) {
            int cantidad = stb_easy_font_print(0.0f, offsetY, (char*)lineas[i].c_str(), NULL, buffer, sizeof(buffer));
            glColor3f(1.0f, 1.0f, 1.0f);
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 16, buffer);
            glDrawArrays(GL_QUADS, 0, cantidad * 4);
            glDisableClientState(GL_VERTEX_ARRAY);
            offsetY += 12.0f;
        }
        glPopMatrix();



        glfwSwapBuffers(window2);
        glfwPollEvents();
        glfwWaitEventsTimeout(1.0 / 60.0);
    }

    glfwDestroyWindow(window2);
}