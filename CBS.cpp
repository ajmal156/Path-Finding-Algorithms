#include<iostream>
#include<vector>
#include<cmath>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

using namespace std;

const int GRID = 4;
float cell = 2.0f / GRID;

struct Node {
    int x, y;
};

bool startSimulation = false; // simulation set key
int step = 0;


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_S && action == GLFW_PRESS)  // Press "s" key to start the simulation
        startSimulation = true;

    if (key == GLFW_KEY_R && action == GLFW_PRESS) // Press " r " key to reset the simulation 
    {
        step = 0;
        startSimulation = false;
    }
}

// PATHS
vector<Node> path1 = {
    {0,1},{0,2},{1,2},{2,2},{3,2}
};

vector<Node> path2 = {
    {1,0},{2,0},{2,1},{2,1},{2,2},{2,3}
};

// START & GOAL
vector<Node> start = { {0,1},{1,0} };
vector<Node> goal = { {3,2},{2,3} };

// GRID
int grid[4][4] = {
    {1,0,0,1},
    {0,0,0,0},
    {0,0,0,0},
    {1,0,0,1},
};

// DRAW CELL
void drawCell(int r, int c, float R, float G, float B) // set the row , column , color value
{
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

// GRID LINES
void drawGridLines()
{
    glColor3f(0, 0, 0);
    glLineWidth(2);

    glBegin(GL_LINES);
    for (int i = 0; i <= GRID; i++)
    {
        float p = -1 + i * cell;

        glVertex2f(p, -1); glVertex2f(p, 1);
        glVertex2f(-1, p); glVertex2f(1, p);
    }
    glEnd();
}

// MAIN
int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(600, 600, "Conflict based Search", NULL, NULL); // window create 
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glViewport(0, 0, 600, 600);  // width, Hight

    glfwSetKeyCallback(window, keyCallback); // Key set

    double lastTime = glfwGetTime();

    float colors[2][3] = {
        {1,0,0}, {0,1,0} // 2 agnet and set color 
    };

    vector<vector<Node>> paths = { path1, path2 };

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // OBSTACLES
        for (int r = 0; r < GRID; r++)
            for (int c = 0; c < GRID; c++)
                if (grid[r][c] == 1)
                    drawCell(r, c, 0, 0, 0);

        for (int i = 0; i < 2; i++) // same color for the start and goal state
        {
            drawCell(start[i].x, start[i].y,
                colors[i][0], colors[i][1], colors[i][2]);

            drawCell(goal[i].x, goal[i].y,
                colors[i][0], colors[i][1], colors[i][2]);
        }
        // AGENTS
        for (int i = 0; i < 2; i++)
        {
            Node a = paths[i][min(step, (int)paths[i].size() - 1)];
            drawCell(a.x, a.y, colors[i][0], colors[i][1], colors[i][2]);
        }

        drawGridLines(); // 

        glfwSwapBuffers(window); // buffer to window  
        glfwPollEvents(); // window show 

        // TIME CONTROL
        if (startSimulation && glfwGetTime() - lastTime > 1.0)
        {
            step++;
            lastTime = glfwGetTime();
        }
    }

    glfwTerminate();
}