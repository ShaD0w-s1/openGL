#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

 }
};

int main(void)
{
	GLFWwindow* window;




	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}


	/* Make the window's context current */
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	std::cout << glGetString(GL_VERSION) << std::endl;

	float postions[6] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			-0.5f,  0.5f
	};

	unsigned int buffer;

	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), postions,GL_STATIC_DRAW);




		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			glBegin(GL_TRIANGLES);

			glDrawArrays(GL_TRIANGLES,);

			glEnd();

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	glfwTerminate();
	return 0;
}