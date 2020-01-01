#include "Snake3D.h"
#include "Snake.h"

#include <GLFW/glfw3.h>

#include <iostream>


/////////////////////////////////////
// Global Variables
// default window params
const int SCR_WIDTH = 600;
const int SCR_HEIGHT = 600;
const char* SCR_TITLE = "Snake3D";

// default game params
int halfGridSizeX = 6;
int halfGridSizeY = 6; // actually the Z axis

Snake* snake = new Snake(0, 0, 3);

// camera
float yCamPos = 13.0f;

// frame timing
static double limitUpdates = 1.0 / 5.0; // limit updates to 5 per second


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

    std::cout << cube << std::endl;
}

void drawGrid() {
    float offset = 0.5f;

    // basic grid/matrix (green lines)
    glColor3ub(24, 221, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(0.15f, 0.75f, 0.15f);

    for (int i = -halfGridSizeX; i <= halfGridSizeX; i++) {
        glVertex3f((float)i - offset, 0 - offset, (float)-halfGridSizeX - offset);
        glVertex3f((float)i - offset, 0 - offset, (float)halfGridSizeX - offset);
    }
    for (int i = -halfGridSizeY; i <= halfGridSizeY; i++) {
        glVertex3f((float)-halfGridSizeY - offset, 0 - offset, (float)i - offset);
        glVertex3f((float)halfGridSizeY - offset, 0 - offset, (float)i - offset);
    }
    glEnd();

    // grid background
    float yOffset = 0.01f; // used to drop the background away from the grid (makes clearer lines)

    glLineWidth(1);
    glColor3ub(30, 85, 80);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(halfGridSizeX - offset, -offset - yOffset, -halfGridSizeY - offset); // front left
    glVertex3f(-halfGridSizeX - offset, -offset - yOffset, -halfGridSizeY - offset); // front right
    glVertex3f(-halfGridSizeX - offset, -offset - yOffset, halfGridSizeY - offset);  // back right
    glVertex3f(halfGridSizeX - offset, -offset - yOffset, halfGridSizeY - offset);  // back left
    glEnd();
}

void display(GLFWwindow* window) {
    // call the resize to set the initial perspective
    onWindowResize(window, SCR_WIDTH, SCR_HEIGHT);

    double lastFrame = glfwGetTime(), timer = lastFrame;
    double deltaTime = 0, currFrame = 0;
    int frames = 0, updates = 0;

    while (!glfwWindowShouldClose(window)) {
        // frame/update timing
        currFrame = glfwGetTime();
        deltaTime += (currFrame - lastFrame) / limitUpdates;
        lastFrame = currFrame;

        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
        // position the camera looking onto the grid
        glLoadIdentity();
        gluLookAt(
            0, yCamPos, -10, // eyeX/Y/Z (eye point)
            0, 0, -1,        // centerX/Y/Z (reference point)
            0, 1, 0          // upX/Y/Z (up vector)
        );

        drawGrid();

        // snakey stuff
        while (deltaTime >= 1.0) {
            snake->updateSnake();
            updates++;
            deltaTime--;
        }

        snake->draw();
        frames++;

        if (glfwGetTime() - timer > 1.0) {
            timer++;
            std::cout << "FPS: " << frames << ", Updates: " << updates << std::endl;
            updates = 0;
            frames = 0;
        }

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
    cubeDL();
    initOpenGL();

    display(window);
}