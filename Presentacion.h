#ifndef PRESENTACION_CLASS_H
#define PRESENTACION_CLASS_H

#include <GLFW/glfw3.h>
#include <iostream>

class presentacion {
public:
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	void drawButton();
	void drawStars();

};


#endif // !PRESENTACION_CLASS_H

#pragma once
