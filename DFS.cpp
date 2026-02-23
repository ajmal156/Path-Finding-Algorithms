#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <stack>
#include <chrono>
#include <thread>
using namespace std;

// Window & Grid
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const int GRID_SIZE = 20;
const float CELL_SIZE = 2.0f / GRID_SIZE;

// Cell types
enum CellState { EMPTY = 0, WALL, START, TARGET, OPEN, CLOSED, PATH };

// Grid data
vector<vector<CellState>> grid(GRID_SIZE, vector<CellState>(GRID_SIZE, EMPTY));
vector<vector<bool>> visited(GRID_SIZE, vector<bool>(GRID_SIZE, false));
vector<vector<glm::ivec2>> parent(GRID_SIZE, vector<glm::ivec2>(GRID_SIZE, { -1,-1 }));

stack<glm::ivec2> dfsStack;

// Start / Target
int startX = 2, startY = 2;        // top-left
int targetX = GRID_SIZE - 2, targetY = GRID_SIZE - 2;     // bottom-right

bool running = false;
bool completed = false;

// Initialize Grid
void initializeGrid() {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = EMPTY;
            visited[i][j] = false;
            parent[i][j] = { -1,-1 };
        }

    grid[startX][startY] = START;
    grid[targetX][targetY] = TARGET;
}

// Reset DFS
void resetDFS() {
    running = false;
    completed = false;

    while (!dfsStack.empty()) dfsStack.pop();

    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++) {
            visited[i][j] = false;
            parent[i][j] = { -1,-1 };

            if (grid[i][j] == OPEN || grid[i][j] == CLOSED || grid[i][j] == PATH)
                grid[i][j] = EMPTY;
        }

    grid[startX][startY] = START;
    grid[targetX][targetY] = TARGET;
}

// Start DFS
void startDFS() {
    resetDFS();
    running = true;
    dfsStack.push({ startX, startY });
}

// Reconstruct Path
void reconstructPath() {
    glm::ivec2 current(targetX, targetY);

    while (current != glm::ivec2(startX, startY)) {
        if (grid[current.x][current.y] != TARGET)
            grid[current.x][current.y] = PATH;

        current = parent[current.x][current.y];
    }
}

// DFS Step
void dfsStep() {
    if (!running || dfsStack.empty() || completed) return;

    glm::ivec2 current = dfsStack.top();
    dfsStack.pop();

    int x = current.x;
    int y = current.y;

    if (visited[x][y]) return;

    visited[x][y] = true;

    if (grid[x][y] != START && grid[x][y] != TARGET)
        grid[x][y] = CLOSED;

    if (x == targetX && y == targetY) {
        completed = true;
        running = false;
        reconstructPath(); // final path show the DFS 
        return;
    }

    int dx[] = { -1,1,0,0 };
    int dy[] = { 0,0,-1,1 };

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE) {

            if (grid[nx][ny] == WALL || visited[nx][ny])
                continue;

            if (parent[nx][ny] == glm::ivec2(-1, -1))
                parent[nx][ny] = { x,y };

            dfsStack.push({ nx, ny });

            if (grid[nx][ny] != TARGET)
                grid[nx][ny] = OPEN;
        }
    }
}

// DRAWING
void drawCell(float x, float y, CellState state) {
    switch (state) {
    case EMPTY:  glColor3f(1, 1, 1); break;
    case WALL:   glColor3f(0.2f, 0.2f, 0.2f); break;
    case START:  glColor3f(0 , 1, 0); break;
    case TARGET: glColor3f(1, 0, 0); break;
    case OPEN:   glColor3f(0.4f, 0.8f, 0.1f); break;
    case CLOSED: glColor3f(0.0f, 0.4f, 0.0f); break;
    case PATH:   glColor3f(0.3f, 0.5f, 0.9f); break;
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
    case GLFW_KEY_SPACE: startDFS(); break;
    case GLFW_KEY_ENTER: dfsStep(); break;
    case GLFW_KEY_A:
        startDFS();
        while (running && !completed) {
            dfsStep();
            glClear(GL_COLOR_BUFFER_BIT);
            drawGrid();
            glfwSwapBuffers(window);
            glfwPollEvents();
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }
        break;
    case GLFW_KEY_R: resetDFS(); break;
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "DFS Visualizer", NULL, NULL);
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
