#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <thread>

// Window & Grid
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const int GRID_SIZE = 20;
const float CELL_SIZE = 2.0f / GRID_SIZE;

// Cell types
enum CellState { EMPTY = 0, WALL, START, TARGET, OPEN, CLOSED, PATH };

// Node structure
struct Node {
    int x, y;
    float g, h, f;
};

struct CompareNode {
    bool operator()(Node const& a, Node const& b) {
        return a.f > b.f; // min-heap
    }
};

// Grid data
std::vector<std::vector<CellState>> grid(GRID_SIZE, std::vector<CellState>(GRID_SIZE, EMPTY));
std::vector<std::vector<bool>> closedSet(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
std::vector<std::vector<float>> gCost(GRID_SIZE, std::vector<float>(GRID_SIZE, 1e9));
std::vector<std::vector<glm::ivec2>> parent(GRID_SIZE, std::vector<glm::ivec2>(GRID_SIZE, { -1,-1 }));

std::priority_queue<Node, std::vector<Node>, CompareNode> openSet;

// Start / Target (TOP-LEFT → BOTTOM-RIGHT)
int startX = 0, startY = GRID_SIZE - 1;       // top-left
int targetX = GRID_SIZE - 2, targetY = 1;    // bottom-right

bool running = false;
bool completed = false;

// Manhattan Heuristic
float heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

void initializeGrid() {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = EMPTY;
            closedSet[i][j] = false;
            gCost[i][j] = 1e9;
            parent[i][j] = { -1,-1 };
        }

    grid[startX][startY] = START;
    grid[targetX][targetY] = TARGET;
}

void resetAStar() {
    running = false;
    completed = false;

    while (!openSet.empty()) openSet.pop();

    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++) {
            closedSet[i][j] = false;
            gCost[i][j] = 1e9;
            parent[i][j] = { -1,-1 };

            if (grid[i][j] == OPEN || grid[i][j] == CLOSED || grid[i][j] == PATH)
                grid[i][j] = EMPTY;
        }

    grid[startX][startY] = START;
    grid[targetX][targetY] = TARGET;
}

void startAStar() {
    resetAStar();
    running = true;

    gCost[startX][startY] = 0;

    Node startNode;
    startNode.x = startX;
    startNode.y = startY;
    startNode.g = 0;
    startNode.h = heuristic(startX, startY, targetX, targetY);
    startNode.f = startNode.g + startNode.h;

    openSet.push(startNode);
}

void reconstructPath() {
    glm::ivec2 current(targetX, targetY);

    while (current != glm::ivec2(startX, startY)) {
        if (grid[current.x][current.y] != TARGET)
            grid[current.x][current.y] = PATH;

        current = parent[current.x][current.y];
    }
}

void aStarStep() {
    if (!running || openSet.empty() || completed) return;

    Node current = openSet.top();
    openSet.pop();

    int x = current.x;
    int y = current.y;

    if (closedSet[x][y]) return;

    closedSet[x][y] = true;

    if (grid[x][y] != START && grid[x][y] != TARGET)
        grid[x][y] = CLOSED;

    if (x == targetX && y == targetY) {
        completed = true;
        running = false;
        reconstructPath();
        return;
    }

    int dx[] = { -1,1,0,0 };
    int dy[] = { 0,0,-1,1 };

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE) {

            if (grid[nx][ny] == WALL || closedSet[nx][ny])
                continue;

            float newG = gCost[x][y] + 1;

            if (newG < gCost[nx][ny]) {
                parent[nx][ny] = { x,y };
                gCost[nx][ny] = newG;

                float h = heuristic(nx, ny, targetX, targetY);
                float f = newG + h;

                openSet.push({ nx, ny, newG, h, f });

                if (grid[nx][ny] != TARGET)
                    grid[nx][ny] = OPEN;
            }
        }
    }
}

// DRAWING
void drawCell(float x, float y, CellState state) {
    switch (state) {
    case EMPTY:  glColor3f(1, 1, 1); break;
    case WALL:   glColor3f(0.2f, 0.2f, 0.2f); break;
    case START:  glColor3f(0.6f, 0.1f, 0.6f); break;
    case TARGET: glColor3f(1, 0, 0); break;
    case OPEN:   glColor3f(0.2f, 0.6f, 1.0f); break;
    case CLOSED: glColor3f(0.6f, 0.2f, 1.0f); break;
    case PATH:   glColor3f(0.0f, 1.0f, 0.0f); break;
    }

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + CELL_SIZE, y);
    glVertex2f(x + CELL_SIZE, y + CELL_SIZE);
    glVertex2f(x, y + CELL_SIZE);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + CELL_SIZE, y);
    glVertex2f(x + CELL_SIZE, y + CELL_SIZE);
    glVertex2f(x, y + CELL_SIZE);
    glEnd();
}

void drawGrid() {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            drawCell(-1 + i * CELL_SIZE, -1 + j * CELL_SIZE, grid[i][j]);
}

// Input
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    switch (key) {
    case GLFW_KEY_SPACE: startAStar(); break;
    case GLFW_KEY_ENTER: aStarStep(); break;
    case GLFW_KEY_A:
        startAStar();
        while (running && !completed) {
            aStarStep();
            glClear(GL_COLOR_BUFFER_BIT);
            drawGrid();
            glfwSwapBuffers(window);
            glfwPollEvents();
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }
        break;
    case GLFW_KEY_R: resetAStar(); break;
    case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true); break;
    }
}

// Mouse
void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int gx = int((xpos / SCR_WIDTH) * GRID_SIZE);
        int gy = int(((SCR_HEIGHT - ypos) / SCR_HEIGHT) * GRID_SIZE);

        if ((gx != startX || gy != startY) &&
            (gx != targetX || gy != targetY))
            grid[gx][gy] = (grid[gx][gy] == WALL) ? EMPTY : WALL;
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "A* Visualizer", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    initializeGrid();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        drawGrid();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
