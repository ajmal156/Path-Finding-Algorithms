#include<iostream>
#include<chrono>
#include<thread>
#include<vector>


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

using namespace std;

const int GRID = 4;
float cell = 2.0f / GRID;

struct Node {
    int x;
    int y;
};

// PATH
vector<Node> path1 = {
    {0,1},{0,2} ,
    {1,2} ,{2,2} , {3,2}
};

vector<Node> path2 = {
   {1,0},{2,0} , {2,1} , {2,1} , {2,2} ,{2,3}
};

 // start state 

vector<Node> start = {
    {0,1},{1,0}
};

// GOAL 
vector<Node> goal = { {3,2} ,{2,3} };

int grid[4][4] = {
    {1,0,0,1},
    {0,0,0,0},
    {0,0,0,0},
    {1,0,0,1},
};

// DRAW CELL
void Drawcell(int r, int c, float R, float G, float B) {
    float x = -1 + c * cell;
    float y = 1 - r * cell;

    glColor3f(R, G, B);

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + cell, y);
    glVertex2f(x + cell, y - cell);
    glVertex2f(x, y - cell);
    glEnd();
}

// DRAW GRID
void drawGridLines() {
    glColor3f(0, 0, 0);
    glLineWidth(2);

    glBegin(GL_LINES);

    for (int i = 0; i < GRID; i++) {
        float p = -1 + i * cell;

        glVertex2f(p, -1);
        glVertex2f(p, 1);

        glVertex2f(-1, p);
        glVertex2f(1, p);

    }
    glEnd();
};





int main() {

    glfwInit();

    GLFWwindow* window = glfwCreateWindow(600, 600, "Micromouse Simulation", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glViewport(0, 0, 600, 600);

    double lastTime = glfwGetTime();
    int step = 0;
    float colors[8][3] = {
        {1,0,0}, {0,1,0},
    };

    while (!glfwWindowShouldClose(window)) {

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // DRAW OBSTACLES
        for (int r = 0; r < GRID; r++) {
            for (int c = 0; c < GRID; c++) {
                if (grid[r][c] == 1) {
                    Drawcell(r, c, 0, 0, 0);
                }
            }
        }

        // Agent 

        vector < vector<Node>>paths = {
            path1 , path2

        };

        // Draw cell 
        for (int i = 0; i <2; i++)
        {
            Node a = paths[i][min(step, (int)paths[i].size() - 1)];
           Drawcell( a.x ,   a.y, colors[i][0], colors[i][1], colors[i][2]);
        }




        // DRAW GRID
        drawGridLines();
        glfwSwapBuffers(window);
        glfwPollEvents();

        // TIME CONTROL
        if (glfwGetTime() - lastTime > 1.0) {
            step++;
            lastTime = glfwGetTime();
        }
    }

    glfwTerminate();
}