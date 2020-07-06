
// Include standard headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


//Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <string>

GLFWwindow* window;

using namespace std;

GLuint renderingProgram;
GLuint VAO;
GLuint VBO;

int numVertices;
int dimVertices;


void printShaderLog(GLuint shader) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        cout << "Shader Info Log: " << log << endl;
        free(log);
    }
}

// displays the contents of OpenGL's log when GLSL linking failed
void printProgramLog(int prog) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        cout << "Program Info Log: " << log << endl;
        free(log);
    }
}

// checks the OpenGL error flag for the occurrence of an OpenGL error
// detects both GLSL compilation errors and OpenGL runtime errors
bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        cout << "glError: " << glErr << endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

string readShaderSource(const char *filePath) {
    string content = "";
    ifstream fileStream(filePath, ios::in);
    string line = "";
    while (!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

GLuint createShaderProgram() {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    string vertShaderStr = readShaderSource("src/vertShader.glsl");
    string fragShaderStr = readShaderSource("src/fragShader.glsl");

    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);

    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1) {
        cout << "vertex compilation failed" << endl;
        printShaderLog(vShader);
    }

    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1) {
        cout << "fragment compilation failed" << endl;
        printShaderLog(fShader);
    }

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);

    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        cout << "linking failed" << endl;
        printProgramLog(vfProgram);
    }

    return vfProgram;
}


void init(){
	renderingProgram = createShaderProgram();

	dimVertices = 6;
	numVertices = 15;

	GLfloat p; // Declarando el parámetro

	//TRIÁNGULO RECTÁNGULO (2do Cuadrante)///////////////////////////////

	glm::vec3 P0(-0.8, 0.8, 0.0); // Punto Inicial P0


	glm::vec3 Q0; // Puntos que se obtendrán de la ecuación de la recta
	glm::vec3 R0;


	p = 5.0f; 	// Parámetro


	// Ecuación de la recta:
	// P = P0 + p * glm::vec3(x,y,z);

	Q0 = P0 + p * glm::vec3(0.0, -0.1, 0.0);
	R0 = P0 + p * glm::vec3(0.1, -0.1, 0.0);


	//TRIÁNGULO ESCALENO (1er Cuadrante)//////////////////////////////////////

	glm::vec3 P1(0.5, 0.8, 0.0); // Punto Inicial P1


	glm::vec3 Q1;
	glm::vec3 R1;

	p = 1.0f;

	Q1 = P1 + p * glm::vec3(-0.1, -0.5, 0.0);
	R1 = P1 + p * glm::vec3(0.4, -0.5, 0.0);


	//TRIÁNGULO ISÓSCELES (3er Caudrante)/////////////////////////////

	glm::vec3 P2(-0.5, -0.2, 0.0); // Punto Inicial P2

	glm::vec3 Q2;
	glm::vec3 R2;

	p = 3.0f;

	Q2 = P2 + p * glm::vec3(-0.1, -0.2, 0.0);
	R2 = P2 + p * glm::vec3(0.1, -0.2, 0.0);


	//RECTÁNGULO (4to Cuadrante) /////////////////////////////////////7

	glm::vec3 P3(0.1, -0.2, 0.0); // Punto Inicial P3

	glm::vec3 Q3;
	glm::vec3 R3;
	glm::vec3 S3; 		// Cuarto punto del rectángulo

	p = 6.0f;

	Q3 = P3 + p * glm::vec3(0.0, -0.1, 0.0);

	p = 8.0f;

	R3 = P3 + p * glm::vec3(0.1, 0.0, 0.0);

	p = 2.0f;

	S3 = P3 + p * glm::vec3(0.4, -0.3, 0.0);


	GLfloat vertices[numVertices * dimVertices] = {

		//Posiciones				//Colores
		P0.x, P0.y, P0.z,		0.3f, 0.8f, 0.9f,
		Q0.x, Q0.y, Q0.z,		0.3f, 0.8f, 0.9f,
		R0.x, R0.y, R0.z,		0.3f, 0.8f, 0.9f,

		P1.x, P1.y, P1.z,		0.9f, 0.9f, 0.0f,
		Q1.x, Q1.y, Q1.z,		0.9f, 0.9f, 0.0f,
		R1.x, R1.y, R1.z,		0.9f, 0.9f, 0.0f,

		P2.x, P2.y, P2.z,		0.10f, 0.25f, 1.00f,
		Q2.x, Q2.y, Q2.z,		0.10f, 0.25f, 1.00f,
		R2.x, R2.y, R2.z,		0.10f, 0.25f, 1.00f,

		P3.x, P3.y, P3.z,		0.6f, 0.0f, 0.1f,
		Q3.x, Q3.y, Q3.z,		0.6f, 0.0f, 0.1f,
		R3.x, R3.y, R3.z,		0.6f, 0.0f, 0.1f,

		Q3.x, Q3.y, Q3.z,		0.6f, 0.0f, 0.1f,		// Reusamos los dos puntos para hacer el rectángulo
		R3.x, R3.y, R3.z,		0.6f, 0.0f, 0.1f,
		S3.x, S3.y, S3.z,		0.6f, 0.0f, 0.1f
	};


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(
		GL_ARRAY_BUFFER,
		dimVertices * numVertices * sizeof(GLfloat),
		vertices,
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(
		0, 						// Layout 0 será para las posiciones
		3,                  	// Dimensiones de los vertices
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(GLfloat),   // Toma los valores del buffer en grupos de 3, pero cada 6 posiciones
		(void*) 0				// Empieza desde el primer elemento
	);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,						// Layout 1 será para los colores
		3,						// Toma r, g y b
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(GLfloat),			// Toma los valores del buffer en grupos de 3, pero cada 6 posiciones
		(GLvoid*) (3 * sizeof(GLfloat))	//Empieza desde el elemento 3 del buffer
	);

	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

}

void display(double currentTime) {
	glClearColor(0.7f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 15);
	glBindVertexArray(0);
}

int main( void ){
	// Initialise GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(800, 800, "Primitivas - RodriguezTocasMiguelAngel", NULL, NULL);

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	init();

	// Check if the window was closed
	while (!glfwWindowShouldClose(window)) {
		display(glfwGetTime());
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Cleanup VBO
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
