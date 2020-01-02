#include "Snake3D.h"
#include "Snake.h"
#include "Food.h"

#include <GLFW/glfw3.h>

#include <iostream>


/////////////////////////////////////
// Global Variables
// default window params
const int SCR_WIDTH = 600;
const int SCR_HEIGHT = 600;
const char* SCR_TITLE = "Snake3D";

// default game params
int halfGridSize = 5;

// game objects
Snake snake(0, 0, 5);
Food* food = new Food(2, 0.5, 3);

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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS)
        return;

    switch (key) {
    case GLFW_KEY_UP:
        snake.move(Direction::DIR_UP);
        break;
    case GLFW_KEY_DOWN:
        snake.move(Direction::DIR_DOWN);
        break;
    case GLFW_KEY_LEFT:
        snake.move(Direction::DIR_LEFT);
        break;
    case GLFW_KEY_RIGHT:
        snake.move(Direction::DIR_RIGHT);
        break;
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, true);
        break;
    }

    snake.detectCollisions(food, halfGridSize);
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
    float offset = 0.5f;

    // basic grid/matrix (green lines)
    glColor3ub(24, 221, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glColor3f(0.15f, 0.75f, 0.15f);

    for (int i = -halfGridSize; i <= halfGridSize + 1; i++) {
        glVertex3f((float)i - offset, 0 - offset, (float)-halfGridSize - offset);
        glVertex3f((float)i - offset, 0 - offset, (float)halfGridSize + 1 - offset);
    }
    for (int i = -halfGridSize; i <= halfGridSize + 1; i++) {
        glVertex3f((float)-halfGridSize - offset, 0 - offset, (float)i - offset);
        glVertex3f((float)halfGridSize + 1 - offset, 0 - offset, (float)i - offset);
    }
    glEnd();

    // grid background
    float yOffset = 0.01f; // used to drop the background away from the grid (makes clearer lines)

    glLineWidth(1);
    glColor3ub(30, 85, 80);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(halfGridSize + 1- offset, -offset - yOffset, -halfGridSize - offset); // front left
    glVertex3f(-halfGridSize - offset, -offset - yOffset, -halfGridSize - offset); // front right
    glVertex3f(-halfGridSize - offset, -offset - yOffset, halfGridSize + 1 - offset);  // back right
    glVertex3f(halfGridSize + 1 - offset, -offset - yOffset, halfGridSize + 1 - offset);  // back left
    glEnd();
}

void gameOver(GLFWwindow* window) {
    std::cout << "*** GAME OVER! ***" << std::endl;
    std::cout << "Your score: " << snake.body.size() << std::endl;

    glfwSetWindowShouldClose(window, true);
}

void display(GLFWwindow* window) {
    // attach callbacks
    glfwSetKeyCallback(window, keyCallback);

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
            snake.updateSnake();
            snake.detectCollisions(food, halfGridSize);

            if (!snake.isAlive())
                gameOver(window);

            updates++;
            deltaTime--;
        }

        snake.draw();
        food->draw();
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
    glEnable(GL_LINE_SMOOTH);
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

    system("pause"); // game should restart at this point
}