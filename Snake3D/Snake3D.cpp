#include "Snake3D.h"
#include "Snake.h"
#include "Food.h"

#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <mmsystem.h>
#include <math.h>
#include <algorithm>
#include <iostream>


/////////////////////////////////////
// Data Structures
enum MapSize {
    MAP_SMALL = 3,
    MAP_MEDIUM,
    MAP_LARGE,
    MAP_HUGE
};


/////////////////////////////////////
// Global Variables
// default window params
const int SCR_WIDTH = 600;
const int SCR_HEIGHT = 600;
const char* SCR_TITLE = "Snake3D";

// default game params
int halfGridSize = MAP_MEDIUM;
bool paused = false;
bool gameStarted = false;
bool gameFinished = false;

// game objects
Snake* snake = new Snake(0, 0, 3);
Food* food = new Food(2, 0.5, 0);

// camera
float yCamPos = 13.0f;

// frame timing
static double limitUpdates = 1.0 / 5.0; // limit updates to 5 per second

// lighting
GLfloat lightPos[] = { 0.0f, 3.0f, -1.0f };
GLfloat lightAmbient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
/////////////////////////////////////


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

    if (key == GLFW_KEY_ESCAPE)
        exit(0);
    else if (key == GLFW_KEY_P) {
        paused = !paused;
        std::cout << (paused ? "PAUSED!" : "RESUMING...") << std::endl;
    }

    if (!paused) {
        switch (key) {
        case GLFW_KEY_UP:
            snake->move(Direction::DIR_UP);
            gameStarted = true;
            break;
        case GLFW_KEY_DOWN:
            snake->move(Direction::DIR_DOWN);
            gameStarted = true;
            break;
        case GLFW_KEY_LEFT:
            snake->move(Direction::DIR_LEFT);
            gameStarted = true;
            break;
        case GLFW_KEY_RIGHT:
            snake->move(Direction::DIR_RIGHT);
            gameStarted = true;
            break;
        }
    }

    snake->detectCollisions(food, halfGridSize);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    if (yCamPos >= 0.5f && yCamPos <= 45.0f)
        yCamPos -= yOffset;
    if (yCamPos <= 0.5f)
        yCamPos = 0.5f;
    if (yCamPos >= 45.0f)
        yCamPos = 45.0f;
}
/////////////////////////////////////


/////////////////////////////////////

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

void gameOver(GLFWwindow* window, bool playerWon) {
    if (playerWon) {
        PlaySoundW(L"sounds/win_sound.wav", NULL, SND_ASYNC);
        std::cout << std::endl << "*** YOU WON! ***" << std::endl;
        std::cout << "Final score: " << snake->body.size() << std::endl;
    }
    else {
        std::cout << std::endl << "*** GAME OVER! ***" << std::endl;
        std::cout << "Your score: " << snake->body.size() << std::endl;
    }

    gameStarted = false;
    gameFinished = true;
}

void display(GLFWwindow* window) {
    // attach callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // call the resize to set the initial perspective
    onWindowResize(window, SCR_WIDTH, SCR_HEIGHT);

    double lastFrame = glfwGetTime(), timer = lastFrame;
    double deltaTime = 0, currFrame = 0;
    int frames = 0, updates = 0;

    while (!gameFinished) {
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

        if (paused || !gameStarted)
            deltaTime = 0;

        // snakey stuff
        while (deltaTime >= 1.0 && !paused && gameStarted) {
            snake->updateSnake();
            snake->detectCollisions(food, halfGridSize);

            if (snake->body.size() == pow(((halfGridSize * 2) + 1), 2))
                gameOver(window, true);

            if (!snake->isAlive())
                gameOver(window, false);

            updates++;
            deltaTime--;
        }

        snake->draw();
        food->draw();
        frames++;

        if (glfwGetTime() - timer > 1.0) {
            timer++;
            //std::cout << "FPS: " << frames << ", Updates: " << updates << std::endl; // DEBUG
            updates = 0;
            frames = 0;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete snake;
    delete food;
}

void startGame(GLFWwindow* window) {
    gameFinished = false;

    snake = new Snake(0, 0, 3);
    food = new Food(2, 0.5, 0);

    display(window);
}

/////////////////////////////////////
// Command Line Prompts
bool promptRematch() {
    std::string response;
    bool responseIsValid = false;

    while (!responseIsValid) {
        std::cout << std::endl << "Would you like to play again? (Y/N)" << std::endl;
        std::cin >> response;

        std::transform(response.begin(), response.end(), response.begin(), ::toupper);

        if (response == "Y" || response == "N")
            responseIsValid = true;
        else
            std::cout << "Error: Unrecognised input" << std::endl;
    }

    return response == "N";
}

MapSize promptMapSize() {
    int size;
    bool responseIsValid = false;

    while (!responseIsValid) {
        // prompt for map size
        std::cout << std::endl << "Please select a map size (enter 1-4):" << std::endl;
        std::cout << "      Small  (1)" << std::endl;
        std::cout << "      Medium (2)" << std::endl;
        std::cout << "      Large  (3)" << std::endl;
        std::cout << "      Huge   (4)" << std::endl;
        std::cout << std::endl;
        std::cin >> size;

        int validSizes[] = { 1, 2, 3, 4 };

        if (std::find(std::begin(validSizes), std::end(validSizes), size) != std::end(validSizes)) {
            responseIsValid = true;
            halfGridSize = size;
        }
        else {
            std::cout << "Error: Invalid selection, please enter a number between 1 and 4" << std::endl;
            std::cin.clear();
            std::cin.ignore(512, '\n');
        }
    }

    switch (size)
    {
    case 1:
        return MapSize::MAP_SMALL;
        break;
    case 2:
        return MapSize::MAP_MEDIUM;
        break;
    case 3:
        return MapSize::MAP_LARGE;
        break;
    case 4:
        return MapSize::MAP_HUGE;
        break;
    default:
        std::cout << "ERROR->" << __FUNCTION__ << ": Expecting an integer between 1 and 4 but got: " << size;
        exit(1);
    }
}
/////////////////////////////////////

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);
}

void printWelcomeAscii() {
    std::cout << R"(
 ###############################################################################
 #          ---_ ......._-_--.                                                 #
 #     (|\ /      / /| \  \                                                    #
 #     /  /     .'  -=-'   `.                                                  #
 #    /  /    .'             )                                                 #
 #  _/  /   .'        _.)   /     _____             _        ____  _____  _    #
 # / o   o        _.-' /  .'     / ____|           | |      |___ \|  __ \| |   #
 # \          _.-'    / .'*|    | (___  _ __   __ _| | _____  __) | |  | | |   #
 #  \______.-'//    .'.' \*|     \___ \| '_ \ / _` | |/ / _ \|__ <| |  | | |   #
 #   \|  \ | //   .'.' _ |*|     ____) | | | | (_| |   <  __/___) | |__| |_|   #
 #    `   \|//  .'.'_ _ _|*|    |_____/|_| |_|\__,_|_|\_\___|____/|_____/(_)   #
 #     .  .// .'.' | _ _ \*|                                                   #
 #     \`-|\_/ /    \ _ _ \*\                                                  #
 #      `/'\__/      \ _ _ \*\                                                 #
 #     /^|            \ _ _ \*                                                 #
 #    '  `             \ _ _ \                                                 #
 #                      \_                                                     #
 ###############################################################################
    )" << std::endl;
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE, NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onWindowResize);

    // setup any required OpenGL settings
    initOpenGL();

    printWelcomeAscii();

    bool rematch = false;

    while (!rematch) {
        // get map size
        SetForegroundWindow(GetConsoleWindow());
        halfGridSize = promptMapSize();

        // begin the game
        glfwShowWindow(window);
        startGame(window);

        // ask for rematch
        SetForegroundWindow(GetConsoleWindow());
        rematch = promptRematch();
    }

    std::cout << std::endl << "Bye!" << std::endl;

    return 0;
}