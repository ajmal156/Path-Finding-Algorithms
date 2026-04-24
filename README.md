# BFS DFS AStar CBS And MAPF Algorithm 

# BFS(Breath First Search)

Breadth-First Search (BFS) is implemented using OpenGL with GLAD, GLFW, and GLM in C++.

The project visually demonstrates how the BFS algorithm works on a 2D grid.

It uses a "queue data structure" based on the FIFO (First In, First Out) principle.

The algorithm explores all neighboring nodes level by level.

It discovers all reachable nodes and selects the shortest path to the target.

The visualization clearly shows node traversal and the final correct path.

Show video Link : https://drive.google.com/file/d/1xpl0OxtDWHJHTEwj76RZBNHQkNpeKzB8/view?usp=drive_link


# DFS(Deapth First Search)

Depth-First Search (DFS) is implemented using OpenGL with GLAD, GLFW, and GLM in C++.

The project visually demonstrates how the DFS algorithm works on a 2D grid.

It uses a "stack data structure" based on the LIFO (Last In, First Out) principle.

The algorithm explores nodes by going as deep as possible before backtracking.

It discovers reachable nodes through recursive or stack-based traversal and finds a path to the target.

The visualization clearly shows the depth-wise traversal process and the final path found by DFS.

Show video Link : https://drive.google.com/file/d/1xpl0OxtDWHJHTEwj76RZBNHQkNpeKzB8/view?usp=drive_link


# AStar
A* (A-Star) pathfinding algorithm in C++ with real-time visual representation using OpenGL. 

The graphics pipeline is built using GLFW, GLAD,and GLM to render a 2D grid environment.

The algorithm logic is developed using C++ library such as priority_queue, vector, algorithm, thread, and chrono for execution and visualization control.

The A* algorithm calculates the optimal path using the cost function.

Formula:

f(n) = g(n) + h(n)

g(n) represents the actual path cost. 

h(n) represents the heuristic estimation cost.

f(n) represents the total cost.

The heuristic is applied to guide the search toward the target efficiently.

The algorithm explores four directional movements (up, down, left, right) 

Continuously selects the node with the minimum f-cost using a priority queue.

This project demonstrates shortest path finding in a 2D grid and provides a clear graphical understanding of how A* works internally.

Show Video Link : https://drive.google.com/file/d/1xxduIEW4af51Ggwde-PjJrEFitwEHYRZ/view?usp=drive_link 


# Conflict-Based Search (CBS) – Multi-Agent Pathfinding

# Project Overview

This project implements the **Conflict-Based Search (CBS)** algorithm for solving the **Multi-Agent Pathfinding (MAPF)** problem.

The goal is to find collision-free paths for multiple agents moving in a grid environment.

In this implementation:

* Grid size: **4 × 4**
* Number of agents: **2**
* Movement: 4-directional (up, down, left, right)
* Constraint handling: Avoid vertex and edge conflicts

#  What is CBS?

Conflict-Based Search (CBS) is a two-level algorithm:

* **High-Level Search:** Resolves conflicts between agents by adding constraints
* **Low-Level Search:** Finds shortest path for each agent (using A*)

CBS ensures:
* Optimal paths
* No collisions between agents


## Problem Setup

### Grid (4×4)

Example coordinate system:

(0,0) → (3,3)

### Agents:

* **Agent 1**

  * Start: (0,1)
  * Goal: (3,2)

* **Agent 2**

  * Start: (1,0)
  * Goal: (2,3)


## Conflict Handling

The algorithm detects:

* **Vertex Conflict** → Two agents in same cell at same time
* **Edge Conflict** → Agents swapping positions

CBS resolves conflicts by:

1. Splitting into two constraint nodes
2. Replanning paths using A*

##  Algorithm Steps

1. Initialize root node with paths for all agents
2. Detect conflicts
3. If no conflict → solution found
4. If conflict:

   * Add constraints
   * Replan affected agent
5. Repeat until conflict-free solution

##  Implementation Details
* Language: C++
* Search Algorithm: A*
* Data Structures:

  * Priority Queue (Open List)
  * Constraint Tree (CT)
  * Path storage per agent
  
##  Learning Outcomes

* Understanding Multi-Agent Pathfinding (MAPF)
* Implementing CBS algorithm
* Handling agent conflicts
* Applying A* in constrained environments

Show video link : https://drive.google.com/file/d/1jN52sjz78XDL8L8P4m9p9XO_D0CvurDJ/view?usp=sharing


# MAPF — Multi-Agent Path Finding (CBS) | 7×7 Grid · 8 Agents

A real-time visualization of the **Multi-Agent Path Finding (MAPF)** problem solved using **Conflict-Based Search (CBS)**. Built with C++ and OpenGL (GLFW + GLAD + GLM).


## What It Does

- Renders a **7×7 grid** with obstacles, start positions, and goal positions
- Simulates **8 agents** moving simultaneously along pre-planned, conflict-free paths
- Each agent waits when needed to avoid collisions — no two agents occupy the same cell at the same time
- All paths are **optimal**: shortest possible routes given the constraints

## Controls

| Key | Action |
|-----|--------|
| `S` | Start the simulation |
| `R` | Reset to the beginning |


## Grid Layout

Obstacles (★) at corners and center cells:
(0,0)  (0,6)
(3,2)  (3,4)
(4,2)  (4,4)
(6,0)  (6,6)

Agents are shown as **colored circles**, start/goal positions as **colored squares** of the same color.


## Agents

| Agent | Start  | Goal   | Color      |
|-------|--------|--------|------------|
| 1     | (0, 1) | (6, 5) | Red        |
| 2     | (1, 0) | (5, 6) | Green      |
| 3     | (2, 0) | (6, 2) | Blue       |
| 4     | (0, 2) | (5, 0) | Yellow     |
| 5     | (0, 3) | (4, 6) | Magenta    |
| 6     | (3, 0) | (6, 4) | Cyan       |
| 7     | (0, 4) | (6, 3) | Purple     |
| 8     | (0, 5) | (3, 6) | Dark Green |

---

## Dependencies

- [GLFW](https://www.glfw.org/) — window and input
- [GLAD](https://glad.dav1d.de/) — OpenGL loader
- [GLM](https://github.com/g-truc/glm) — math library

---

## Build & Run

```bash
g++ main.cpp -o mapf -lglfw -lGL -ldl -pthread
./mapf
```

Or with CMake:

''bash
mkdir build && cd build
cmake ..
make
./mapf
``

Press **S** to start the simulation after the window opens.

---

## Algorithm

Each agent's path is pre-computed using **CBS (Conflict-Based Search)**:
- Agents follow shortest paths by default
- When a conflict is detected, one agent is assigned a **wait step** at its current position
- This repeats until all paths are conflict-free

The result is an optimal, collision-free joint plan across all 8 agents.

Research Paper : https://drive.google.com/file/d/1j_iq-lZa0SQvfkJsfHqvNN3iGfOflGRM/view?usp=sharing

Show Video link : https://drive.google.com/file/d/1CA9BUP-qGGvBpFcpOTYZoYOjEDnK9OXH/view?usp=sharing

