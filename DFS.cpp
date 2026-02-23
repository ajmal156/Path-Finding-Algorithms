#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

using namespace std;

#include<stack>
#include<vector>
#include<chrono>
#include<thread>

// assigned the value 
const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 800;
const int GRID_SIZE = 20;
const float CELLSTATE = 2.0f / GRID_SIZE;



int main() {

	glfwInit();

	glfwWindowHint(GLFW_VERSION_MAJOR ,3);
	glfwWindowHint(GLFW_VERSION_MINOR ,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_COMPAT_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH , SCR_HEIGHT ,"DFS USING OPENGL" ,NULL , NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);


	while (!glfwWindowShouldClose(window)){
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;

}