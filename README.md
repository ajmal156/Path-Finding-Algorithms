# BFS DFS AStar And CBS Algorithm 

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



