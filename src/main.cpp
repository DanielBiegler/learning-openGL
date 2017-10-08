#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(int argc, char *argv[])
{
	GLFWwindow *window;

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
	glfwMakeContextCurrent(window);

	/* Initialize GLEW  so we can use modern OpenGL */
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Something went wrong with GLEW.\nExiting program now." << std::endl;
		return -1;
	}

	/* Print the current OpenGL version */
	std::cout << glGetString(GL_VERSION) << std::endl;

	/* Create a buffer for the triangle */
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	/* Bind it so the next draw call will act on this buffer */
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	/* Create the indices for the triangle and set the data inside the buffer */
	float position[6] = {-0.5f, -0.5f,
						 0.0f, 0.5f,
						 0.5f, -0.5f};
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* We want to draw TRIANGLES, 
		 * start from index 0 and 
		 * want to draw 3 indices in total 
		 */
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}