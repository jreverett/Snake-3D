#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <iostream>


/////////////////////////////////////
// Forward Declarations


/////////////////////////////////////
// Global Variables
// default window params
const int SCR_WIDTH = 600;
const int SCR_HEIGHT = 600;
const char* SCR_TITLE = "Snake3D";

// default game params
int halfGridSizeX = 5;
int halfGridSizeY = 5;

// camera
float yCamPos = 13.0f;


/////////////////////////////////////
// Callbacks & Listeners
void onWindowResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    
    // reset the perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = width * 1.0f / height;
    gluPerspective(50, aspectRatio, 1, 250);
    glMatrixMode(GL_MODELVIEW);
}


/////////////////////////////////////
// Display Lists
// creates a simple cube
void cubeDL() {
    GLuint cube = glGenLists(1);
    glNewList(cube, GL_COMPILE);
        glBegin(GL_QUADS);
        // front face
        glNormal3f(0, 0, -1);
        glVertex3f(0.5, 0.5, -0.5); 
        glVertex3f(-0.5, 0.5, -0.5);
        glVertex3f(-0.5, -0.5, -0.5);
        glVertex3f(0.5, -0.5, -0.5);
        // back face
        glNormal3f(0, 0, 1);
        glVertex3f(0.5, 0.5, 0.5);
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(-0.5, -0.5, 0.5);
        glVertex3f(0.5, -0.5, 0.5);
        // top face
        glNormal3f(0, 1, 0);
        glVertex3f(-0.5, 0.5, -0.5);
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(0.5, 0.5, 0.5);
        glVertex3f(0.5, 0.5, -0.5);
        // bottom face
        glNormal3f(0, -1, 0);
        glVertex3f(-0.5, -0.5, -0.5);
        glVertex3f(-0.5, -0.5, 0.5);
        glVertex3f(0.5, -0.5, 0.5);
        glVertex3f(0.5, -0.5, -0.5);
        // left face
        glNormal3f(1, 0, 0);
        glVertex3f(0.5, 0.5, -0.5);
        glVertex3f(0.5, 0.5, 0.5);
        glVertex3f(0.5, -0.5, 0.5);
        glVertex3f(0.5, -0.5, -0.5);
        // right face
        glNormal3f(-1, 0, 0);
        glVertex3f(-0.5, 0.5, -0.5);
        glVertex3f(-0.5, 0.5, 0.5);
        glVertex3f(-0.5, -0.5, 0.5);
        glVertex3f(-0.5, -0.5, -0.5);
        glEnd();
    glEndList();
}


void drawGrid() {
    // basic grid/matrix (green lines)
    glColor3ub(24, 221, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(0.15f, 0.75f, 0.15f);

    for (int i = -halfGridSizeX; i <= halfGridSizeX; i++) {
        glVertex3f((float)i, 0, (float)-halfGridSizeX);
        glVertex3f((float)i, 0, (float)halfGridSizeX);
    }
    for (int i = -halfGridSizeY; i <= halfGridSizeY; i++) {
        glVertex3f((float)-halfGridSizeY, 0, (float)i);
        glVertex3f((float)halfGridSizeY, 0, (float)i);
    }
    glEnd();

    // grid background
    glLineWidth(1);
    glColor3ub(30, 85, 80);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-5 - 0.5, -0.51, -6.5);
    glVertex3f(-5 - 0.5, -0.51, 6.5);
    glVertex3f(6 - 0.5, -0.51, 6.5);
    glVertex3f(6 - 0.5, -0.51, -6.5);
    glEnd();
}

void display(GLFWwindow* window) {
    // call the resize to set the initial perspective
    onWindowResize(window, SCR_WIDTH, SCR_HEIGHT);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
        // position the camera looking onto the grid
        glLoadIdentity();
        gluLookAt(
            0, yCamPos, -5, // eyeX/Y/Z (eye point)
            0, 0, 0,        // centerX/Y/Z (reference point)
            0, 1, 0         // upX/Y/Z (up vector)
        );

        drawGrid();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE, NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onWindowResize);
    
    // setup any required OpenGL settings
    initOpenGL();

    display(window);
}