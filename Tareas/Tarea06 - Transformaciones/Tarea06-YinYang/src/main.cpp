//============================================================================
// Name        :	Ying y Yang
// Student     :	Rodriguez Tocas, Miguel Angel
// Version     :
// Description :
//============================================================================

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

const float toRadians = 3.14159265f / 180.0f;

GLuint renderingProgram;

GLfloat* m_Vertices;
int n_Vertices;
GLuint m_VBO;
GLuint m_VAO;

float curAngle = 0.0f;
float offset = 0.1f;
float increment = 0.1f;

void init (GLFWwindow* window) {

	renderingProgram = Utils::createShaderProgram("src/vertShader.glsl", "src/fragShader.glsl");

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	GLfloat radius = 1;
	GLint numberOfSides = 150;
	n_Vertices = numberOfSides+1 +1; // puntos + 1 punto central + 1 punto repetido para cerrar la figura
	GLfloat twicePi = 2.0f * M_PI;

	GLfloat verticesX[n_Vertices];
	GLfloat verticesY[n_Vertices];
	GLfloat verticesZ[n_Vertices];

	verticesX[0] = 0;
	verticesY[0] = 0;
	verticesZ[0] = 0;

	// Aquí se generan los vertices que formaran la circuferencia
	// Los vetices empiezan a generarse a partir del punto (0.0, 1.0) o a PI/2 de rotacion
	for (int i = 1; i < n_Vertices; i++) {
		verticesX[i] = (radius * cos((i-1) * twicePi / (numberOfSides) + twicePi/4));
		verticesY[i] = (radius * sin((i-1) * twicePi / (numberOfSides) + twicePi/4));
		verticesZ[i] = 0;
	}

	GLfloat m_Vertices[(n_Vertices) * 3];

	for (int i = 0; i < n_Vertices; i++) {
		m_Vertices[i * 3] = verticesX[i];
		m_Vertices[i * 3 + 1] = verticesY[i];
		m_Vertices[i * 3 + 2] = verticesZ[i];
	}

	glGenBuffers(1, &m_VBO);

	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(
			GL_ARRAY_BUFFER,
			(n_Vertices) * 3 * sizeof(GLfloat),
			m_Vertices,
			GL_STATIC_DRAW
			);


	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*) 0
	);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {

	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.3f, 0.8f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);
	glBindVertexArray(m_VAO);

	// se obtienen los uniforms del vertex y el fragment shader
	GLuint uniformColor = glGetUniformLocation(renderingProgram, "u_color");
	GLuint uniformModel = glGetUniformLocation(renderingProgram, "u_model");


	//INCREMENTARÁ O DECREMENTARÁ EL ANGULO DE ROTACION
	//DEPENDIENDO DE LA FLECHA PRESIONADA
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		increment+=0.05f;
	} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if(increment>=0){
			increment-=0.1f;
		}
	}

	curAngle -= increment;
	if (curAngle <= -360)
	{
		curAngle += 360;
	}

	// PINTA CIRCULO NEGRO MAYOR
	glm::mat4 model(1.0f);

	glUniform3f(uniformColor, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, n_Vertices);


	// PINTA SEMICIRCULO BLANCO MAYOR
	model = glm::mat4 (1.0f);
	glUniform3f(uniformColor, 1.0, 1.0, 1.0);

	model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.98f,0.98f,1.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, n_Vertices/2 + 1);


	//PINTA CIRCULO NEGRO MEDIO
	model = glm::mat4 (1.0f);
	model = glm::scale(model, glm::vec3(0.49f,0.49f,1.0f));
	model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(uniformColor, 0.0, 0.0, 0.0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, n_Vertices);


	//PINTA CIRCULO BLANCO MEDIO
	model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(uniformColor, 1.0, 1.0, 1.0);

	glDrawArrays(GL_TRIANGLE_FAN, 0, n_Vertices);


	// PINTA CIRCULO BLANCO PEQUEÑO
	model = glm::mat4 (1.0f);
	model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, -0.49f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f,0.1f,1.0f));

	glUniform3f(uniformColor, 1.0, 1.0, 1.0);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLE_FAN, 0, n_Vertices);


	// PINTA CIRCULO NEGRO PEQUEÑO
	model = glm::mat4 (1.0f);
	model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.49f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f,0.1f,1.0f));

	glUniform3f(uniformColor, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLE_FAN, 0, n_Vertices);


	glBindVertexArray(0);
	glUseProgram(0);
}

int main(void) {
    if (!glfwInit()) {
    	exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

    GLFWwindow* window = glfwCreateWindow(800, 800, "YinYang - Rodriguez Tocas", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
    	exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
