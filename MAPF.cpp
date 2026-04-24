#include<iostream>
#include<vector>
#include<thread>
#include<chrono>
#include<cmath>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

using namespace std;

const int GRID = 7;
float cell = 2.0f / GRID;
const float PI = 3.14159265f;

struct Node { 
    int x;
    int y; 
};

bool startSimulation = false; //  simulation start key 
int step = 0;


void keyCallback(GLFWwindow* window, int key , int scanecode , int action , int mods) {

    if (key == GLFW_KEY_S && action == GLFW_PRESS) { // user an enter the " s " key simulation will startig

        startSimulation = true;

    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) { // user can enter the "r" key program will reset

        step = 0;
        startSimulation = false;
    }
}


// PATHS
vector<Node> path1 = {
 {0,1},{0,1}, // Agent Waiting Time
 {1,1},
 {2,1},{2,1}, // Agent Waiting Time
 {3,1},{4,1},{5,1},{5,2},{5,3},{5,4},{5,5},{6,5}
};
vector<Node> path2 = {
 {1,0},{1,1},{2,1},
 {3,1},{3,1}, // Agent Waiting time
 {4,1},{5,1},{5,2},{5,3},{5,4},{5,5},{5,6}
};
vector<Node> path3 = {
 {2,0},{2,1},{3,1},
 {4,1},{4,1}, // Agent Waiting time
 {5,1},{6,1},{6,2}
};
vector<Node> path4 = {
 {0,2},{0,2},// Agent Waiting Time
 {1,2},{1,2}, // Agent Waiting Time 
 {1,1},{2,1},{3,1},{4,1},{5,1},{5,0}
};
vector<Node> path5 = {
 {0,3},{1,3},{2,3},{2,4},
 {2,5},{3,5},{4,5},{4,6}
};
vector<Node> path6 = {
 {3,0},{4,0},{4,1},
 {5,1},{5,1},// Agent wating time
 {5,2},{5,2}, // Agent wating time
 {5,3},{5,4},{6,4}
};
vector<Node> path7 = {
 {0,4},{1,4},{1,3},{2,3},
 {3,3},{4,3},{5,3},{6,3}
};
vector<Node> path8 = {
 {0,5},{1,5},
 {2,5},{2,5},// Agent wating time 
 {2,6},{3,6}
};

vector<Node> start = { // Start State Agent Position
    {0,1},{1,0},{2,0},{0,2},
    {0,3},{3,0},{0,4},{0,5}
};
vector<Node> goals = { // Goal State Agent Position 
    {6,5},{5,6},{6,2},{5,0},
    {4,6},{6,4},{6,3},{3,6}
};

int grid[7][7] = {
 {1,0,0,0,0,0,1},
 {0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0},
 {0,0,1,0,1,0,0},
 {0,0,1,0,1,0,0},
 {0,0,0,0,0,0,0},
 {1,0,0,0,0,0,1}
};                 // Grid Structure 


//  grid lines
void drawGridLines()
{
    glColor3f(0, 0, 0);
    glLineWidth(1);
    glBegin(GL_LINES);
    for (int i = 0; i <= GRID; i++)
    {
        float p = -1 + i * cell;
        glVertex2f(p, -1);
        glVertex2f(p, 1);
        glVertex2f(-1, p);
        glVertex2f(1, p);
    }
    glEnd();
}


// filled circle 
void drawCircle(float cx, float cy, float radius, float R, float G, float B, int segs = 40)
{
    glColor3f(R, G, B);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segs; ++i)
    {
        float a = 2.0f * PI * i / segs;
        glVertex2f(cx + radius * cosf(a), cy + radius * sinf(a));
    }
    glEnd();
}



//  OBSTACLE: small black star 
void drawStar(int r, int c) // Draw the circle for the circle in the grid 
{
    float cx = -1.0f + c * cell + cell * 0.5f;
    float cy = 1.0f - r * cell - cell * 0.5f;
    float outerR = cell * 0.28f;   // smaller than before
    float innerR = cell * 0.11f;
    int   points = 5;

    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(cx, cy);
    for (int i = 0; i <= points * 2; ++i)
    {
        float radius = (i % 2 == 0) ? outerR : innerR;
        float angle = -PI / 2.0f + i * (PI / points);
        glVertex2f(cx + radius * cosf(angle),
            cy + radius * sinf(angle));
    }
    glEnd();
}

// HALF SQUARE: centred in cell, agent colour 
//  Used for both START and GOAL — same shape, same size
void drawHalfSquare(int r, int c, float aR, float aG, float aB)
{
    float cx = -1.0f + c * cell + cell * 0.5f;
    float cy = 1.0f - r * cell - cell * 0.5f;
    float half = cell * 0.25f;   // square is half the cell size

    glColor3f(aR, aG, aB);
    glBegin(GL_QUADS);
    glVertex2f(cx - half, cy + half);
    glVertex2f(cx + half, cy + half);
    glVertex2f(cx + half, cy - half);
    glVertex2f(cx - half, cy - half);
    glEnd();

    // dark outline
    glColor3f(aR * 0.45f, aG * 0.45f, aB * 0.45f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx - half, cy + half);
    glVertex2f(cx + half, cy + half);
    glVertex2f(cx + half, cy - half);
    glVertex2f(cx - half, cy - half);
    glEnd();
}

//  AGENT: small filled circle in agent colour 
void drawAgentCircle(int r, int c, float aR, float aG, float aB)
{
    float cx = -1.0f + c * cell + cell * 0.5f;
    float cy = 1.0f - r * cell - cell * 0.5f;
    float radius = cell * 0.28f;   // small circle

    drawCircle(cx, cy, radius, aR, aG, aB);

    // outline
    glColor3f(aR * 0.45f, aG * 0.45f, aB * 0.45f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 40; ++i)
    {
        float a = 2.0f * PI * i / 40;
        glVertex2f(cx + radius * cosf(a), cy + radius * sinf(a));
    }
    glEnd();
}


// MAIN 
int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(600, 600, "MAPF CBS (8 Agents)", NULL, NULL); // Show window width and hight and Project name 

    glfwMakeContextCurrent(window);  

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    
    glViewport(0, 0, 600, 600);

    glfwSetKeyCallback(window, keyCallback); // Key set

    double lastTime = glfwGetTime();

    float colors[8][3] = {
        {1,0,0}, {0,1,0}, {0,0,1}, {1,1,0},
        {1,0,1}, {0,1,1}, {0.5f,0.2f,0.8f}, {0.3f,0.6f,0.1f} // color for the 8 Agents
    };

    while (!glfwWindowShouldClose(window)) // Main loop 
    {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // OBSTACLES  small black star
        for (int r = 0; r < GRID; r++)
            for (int c = 0; c < GRID; c++)
                if (grid[r][c] == 1)
                    drawStar(r, c);

        // START  half square
        for (int i = 0; i < 8; i++)
            drawHalfSquare(start[i].x, start[i].y,
                colors[i][0], colors[i][1], colors[i][2]);

        // GOAL  half square (same shape, same size as start)
        for (int i = 0; i < 8; i++)
            drawHalfSquare(goals[i].x, goals[i].y,
                colors[i][0], colors[i][1], colors[i][2]);

        // AGENTS  small circle
        vector<vector<Node>> paths = {
            path1,path2,path3,path4,
            path5,path6,path7,path8  // Full path 
        };

        for (int i = 0; i < 8; i++)
        {
            Node a = paths[i][min(step, (int)paths[i].size() - 1)];
            drawAgentCircle(a.x, a.y,
                colors[i][0], colors[i][1], colors[i][2]);
        }

        drawGridLines();

        glfwSwapBuffers(window); // Draw buffer to window 
        glfwPollEvents(); // window resize 

        if (startSimulation && glfwGetTime() - lastTime > 1.0)
        {
            step++;
            lastTime = glfwGetTime(); // delay the time 
        }
    }

    glfwTerminate(); // Close the terminal
}
