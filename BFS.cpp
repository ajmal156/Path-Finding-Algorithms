 // BFS Visualization with GLFW, GLAD, GLM
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>
#include <thread>
using namespace std;

// Window & Grid
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const int GRID_SIZE = 20;
const float CELL_SIZE = 2.0f / GRID_SIZE;

// Cell Types
enum CellState { EMPTY = 0, WALL, START, TARGET, VISITED, PATH };

// Grid data using 2D vector 
vector<vector<CellState>> grid(GRID_SIZE, vector<CellState>(GRID_SIZE, EMPTY));
vector<vector<bool>> visited(GRID_SIZE, vector<bool>(GRID_SIZE, false));
vector<vector<glm::ivec2>> parent(GRID_SIZE, vector<glm::ivec2>(GRID_SIZE, { -1,-1 }));

queue<glm::ivec2> bfsQueue;
vector<glm::ivec2> bfsPath;

// Start / Target and set the position x-axis , y-axis in 20 by 20 Grid  
int startX = 2, startY = 2;
int targetX = GRID_SIZE - 3, targetY = GRID_SIZE - 3;
bool bfsRunning = false, bfsCompleted = false;

// Initialize Grid and Check The Grid  
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

// Reset
void resetBFS() {
    bfsRunning = false; bfsCompleted = false;
    while (!bfsQueue.empty()) bfsQueue.pop();
    bfsPath.clear();

    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++) {
            visited[i][j] = false;
            parent[i][j] = { -1,-1 };
            if (grid[i][j] == VISITED || grid[i][j] == PATH)
                grid[i][j] = EMPTY;
        }
    grid[startX][startY] = START;
    grid[targetX][targetY] = TARGET;
}

// Start BFS Check the Node UP , DOWN , RIGHT , LEFT , Searching
void startBFS() {
    resetBFS();
    bfsRunning = true;
    visited[startX][startY] = true;
    bfsQueue.push({ startX,startY });
}

// One BFS step
void bfsStep() {
    if (!bfsRunning || bfsQueue.empty() || bfsCompleted) return;

    glm::ivec2 current = bfsQueue.front();
    bfsQueue.pop();
    int x = current.x, y = current.y;

    if (x == targetX && y == targetY) {
        bfsCompleted = true;
        bfsRunning = false;

        // Build path
        glm::ivec2 c(targetX, targetY);
        while (c != glm::ivec2(startX, startY)) {
            bfsPath.push_back(c);
            c = parent[c.x][c.y];
        }
        bfsPath.push_back({ startX,startY });
        std::reverse(bfsPath.begin(), bfsPath.end());

        for (auto& cell : bfsPath) {
            if (grid[cell.x][cell.y] != START && grid[cell.x][cell.y] != TARGET)
                grid[cell.x][cell.y] = PATH;
        }
        return;
    }

    if (grid[x][y] != START)
        grid[x][y] = VISITED;

    int dx[] = { -1,1,0,0 };
    int dy[] = { 0,0,-1,1 };

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i], ny = y + dy[i];
        if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE) {
            if (grid[nx][ny] != WALL && !visited[nx][ny]) {
                visited[nx][ny] = true;
                parent[nx][ny] = { x,y };
                bfsQueue.push({ nx,ny });
            }
        }
    }
}

// Draw cell
void drawCell(float x, float y, CellState state) {
    switch (state) {
    case EMPTY:   glColor3f(1, 1, 1); break; 
    case WALL:    glColor3f(0.2f, 0.2f, 0.2f); break;
    case START:   glColor3f(0.2f, 0.5f, 0.7f); break;
    case TARGET:  glColor3f(1, 0, 0); break;
    case VISITED: glColor3f(0.6f, 0.9f, 0.8f); break;
    case PATH:    glColor3f(0.9, 0.9, 0.9); break;
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

// Draw Grid
void drawGrid() {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            drawCell(-1 + i * CELL_SIZE, -1 + j * CELL_SIZE, grid[i][j]);
}

// Keyboard User Enter the Button Start  
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    switch (key) {
    case GLFW_KEY_SPACE: startBFS(); break;
    case GLFW_KEY_ENTER: bfsStep(); break;
    case GLFW_KEY_A:
        startBFS();
        while (bfsRunning && !bfsCompleted) {
            bfsStep();
            glClear(GL_COLOR_BUFFER_BIT);
            drawGrid();
            glfwSwapBuffers(window);
            glfwPollEvents();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        break;
    case GLFW_KEY_R: resetBFS(); break;
    case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true); break;
    }
}

// Mouse for walls
void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int gx = int((xpos / SCR_WIDTH) * GRID_SIZE);
        int gy = int(((SCR_HEIGHT - ypos) / SCR_HEIGHT) * GRID_SIZE);

        if (gx >= 0 && gx < GRID_SIZE && gy >= 0 && gy < GRID_SIZE) {
            if ((gx != startX || gy != startY) && (gx != targetX || gy != targetY))
                grid[gx][gy] = (grid[gx][gy] == WALL) ? EMPTY : WALL;
        }
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "BFS Visualizer", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    initializeGrid();

    cout << "SPACE = Start BFS\n ENTER = Step\n A = Auto\n R = Reset\n Mouse = Walls\n";

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        drawGrid();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}
