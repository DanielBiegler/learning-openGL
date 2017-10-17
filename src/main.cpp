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

	/* we have to explicitly enable the vertex attribute array
	 * since its disabled by default. we only have a single one so we can pass '0'.
	 */
	glEnableVertexAttribArray(0);
	/* 0. index: we only have one attribute(positions) so we can start from 0
	 * 1. size: component count aka 2 floats per attribute
	 * 2. type: we use floats, so use GL_FLOAT
	 * 3. normalized: our floats are already normalized(0..1) so GL_FALSE
	 * 4. stride: amount of bytes between each vertex, 2 floats = 8 bytes
	 * 5. pointer: this is the first and only attribute, so use 0
	 */
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

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