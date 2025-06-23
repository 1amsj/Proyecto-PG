#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Sky.h"
#include "Uso.h"
#include "Soon.h"
#include <iostream>
#include <cmath>
#include "stb_easy_font.h"
#include <vector>
#include <string>
#include <sstream>

float boxX1 = -0.9f, boxY1 = 0.8f, boxWidth1 = 1.8f, boxHeight1 = 1.5f, escalaX1 = 0.008f, escalaY1 = 0.012f, margenX1 = 0.03f, margenY1 = 0.05f;
float maxWidth1 = (boxWidth1 - 2 * margenX1);
char buffer1[10000];

std::vector<std::string> lineSeparator1(const char* texto, float maxWidth, float escalaX) {
    std::vector<std::string> lineas;
    std::istringstream stream(texto);
    std::string palabra, linea;

    while (stream >> palabra) {
        std::string temp = linea.empty() ? palabra : linea + " " + palabra;

        float ancho = temp.size() * 4.5f * escalaX;

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

void drawButton1(float x, float y, float width, float height, const char* text) {
    float radius = 0.05f;
    int segments = 16;

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x + radius, y);
    glVertex2f(x + width - radius, y);
    glVertex2f(x + width - radius, y - height);
    glVertex2f(x + radius, y - height);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(x, y - radius);
    glVertex2f(x + radius, y - radius);
    glVertex2f(x + radius, y - height + radius);
    glVertex2f(x, y - height + radius);
    glVertex2f(x + width - radius, y - radius);
    glVertex2f(x + width, y - radius);
    glVertex2f(x + width, y - height + radius);
    glVertex2f(x + width - radius, y - height + radius);
    glEnd();

    // Esquinas 
    glBegin(GL_TRIANGLE_FAN); // Superior izquierda
    glVertex2f(x + radius, y - radius);
    for (int i = 90; i <= 180; i += 10)
        glVertex2f(x + radius + cos(i * 3.1416 / 180.0f) * radius,
            y - radius + sin(i * 3.1416 / 180.0f) * radius);
    glEnd();

    glBegin(GL_TRIANGLE_FAN); // Inferior izquierda
    glVertex2f(x + radius, y - height + radius);
    for (int i = 180; i <= 270; i += 10)
        glVertex2f(x + radius + cos(i * 3.1416 / 180.0f) * radius,
            y - height + radius + sin(i * 3.1416 / 180.0f) * radius);
    glEnd();

    glBegin(GL_TRIANGLE_FAN); // Inferior derecha
    glVertex2f(x + width - radius, y - height + radius);
    for (int i = 270; i <= 360; i += 10)
        glVertex2f(x + width - radius + cos(i * 3.1416 / 180.0f) * radius,
            y - height + radius + sin(i * 3.1416 / 180.0f) * radius);
    glEnd();

    glBegin(GL_TRIANGLE_FAN); // Superior derecha
    glVertex2f(x + width - radius, y - radius);
    for (int i = 0; i <= 90; i += 10)
        glVertex2f(x + width - radius + cos(i * 3.1416 / 180.0f) * radius,
            y - radius + sin(i * 3.1416 / 180.0f) * radius);
    glEnd();

    // Texto centrado
    glPushMatrix();
    glTranslatef(x + width / 2.0f, y - height / 2.0f + 0.01f, 0.0f);
    glScalef(0.007f, -0.01f, 1.0f);
    int offsetX = -(int(strlen(text)) * 4) + 12;
    int len = stb_easy_font_print(offsetX, 0.0f, (char*)text, NULL, buffer1, sizeof(buffer1));
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer1);
    glDrawArrays(GL_QUADS, 0, len * 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}




bool mouseOverButton1(double mouseX, double mouseY, float btnX, float btnY, float btnW, float btnH) {
    return mouseX >= btnX && mouseX <= (btnX + btnW) &&
        mouseY >= (btnY - btnH) && mouseY <= btnY;
}

void drawTransparentBoxWithBorder1(float x, float y, float width, float height, float r, float g, float b, float alpha) {
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

void Sky::drawSky() {
    GLFWwindow* window2 = glfwCreateWindow(1000, 800, "¿Como se usa?", NULL, NULL);
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
    Uso uso;
    Soon soon;

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

        drawTransparentBoxWithBorder1(boxX1, boxY1, boxWidth1, boxHeight1, 0.1f, 0.1f, 0.1f, 0.6f);

        const char* texto1 = "La casa puede rotarse para observarla desde distintos angulos : Con el mouse :"
            "Manten presionado el botón izquierdo y mueve el mouse para girar la vista alrededor de la casa."
            "Con las teclas W, A, S, D :  Usa estas teclas para ajustar la rotacion vertical y horizontal, permitiendo "
            "ver la casa desde diferentes perspectivas.";

        // Obtener líneas ajustadas
        std::vector<std::string> lineas = lineSeparator1(texto1, maxWidth1, escalaX1);

        float offsetY = 0.0f;

        // Dibujar líneas
        glPushMatrix();
        glTranslatef(boxX1 + 0.03f, boxY1 - 0.15f, 0.0f);
        glScalef(0.006f, -0.009f, 1.0f);

        for (int i = 0; i < lineas.size(); ++i) {
            int cantidad = stb_easy_font_print(0.0f, offsetY, (char*)lineas[i].c_str(), NULL, buffer1, sizeof(buffer1));
            glColor3f(1.0f, 1.0f, 1.0f);
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 16, buffer1);
            glDrawArrays(GL_QUADS, 0, cantidad * 4);
            glDisableClientState(GL_VERTEX_ARRAY);
            offsetY += 12.0f;
        }
        glPopMatrix();

        const char* btnText = "Proyecto";

        // Calcula el ancho del texto en coordenadas OpenGL
        float textWidth = strlen(btnText) * 8.0f * 0.007f;  // 8 píxeles por carácter * escala
        float textHeight = 0.15f;  // altura estimada

        float btnW = textWidth;
        float btnH = textHeight + 0.02f;

        float btnX = -btnW / 2.0f;
        float btnY = -0.75f;

        drawButton1(btnX, btnY, btnW, btnH, btnText);



        // Manejo del clic
        if (glfwGetMouseButton(window2, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double mouseX, mouseY;
            glfwGetCursorPos(window2, &mouseX, &mouseY);
            int winW, winH;
            glfwGetWindowSize(window2, &winW, &winH);

            float normX = (mouseX / winW) * 2.0f - 1.0f;
            float normY = 1.0f - (mouseY / winH) * 2.0f;

            if (mouseOverButton1(normX, normY, btnX, btnY, btnW, btnH)) {
                glfwSetWindowShouldClose(window2, GLFW_TRUE);
                soon.drawWindow2();
            }
        }

        glfwSwapBuffers(window2);
        glfwPollEvents();
        glfwWaitEventsTimeout(1.0 / 60.0);
    }

    glfwDestroyWindow(window2);
}
