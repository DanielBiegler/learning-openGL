#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

struct Shader_program_source
{
	std::string vertex_source;
	std::string fragment_source;
};

static Shader_program_source parse_shader_file(const std::string &filepath)
{
	enum class Shader_type
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	/* file content of shader file */
	std::ifstream stream(filepath);
	/* current working line */
	std::string line;
	/* one stream for vertex and one for fragment shader */
	std::stringstream strstream[2];
	/* current read shaderype */
	Shader_type shader_type = Shader_type::NONE;
	while (getline(stream, line))
	{
		std::cout << line << std::endl;

		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				shader_type = Shader_type::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				shader_type = Shader_type::FRAGMENT;
			}
		}
		else
		{
			strstream[(int)shader_type] << line << '\n';
		}
	}

	return {strstream[0].str(), strstream[1].str()};
}

static unsigned int compile_shader(unsigned int type, const std::string &source)
{
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	/* 0. shader: the id of the shader
	 * 1. count: how many shaders
	 * 2. string: pointer to the pointer of the source
	 * 3. length: passing null makes that opengl assumes the source is null-terminated
	 */
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	/* Error checking */
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *msg = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, msg);
		std::cout << "Failed to compile shader of type: " << type << std::endl;
		std::cout << msg << std::endl;

		glDeleteShader(id);
		return GL_FALSE;
	}

	return id;
}

static unsigned int create_shader(const std::string &vertex_shader, const std::string &fragment_shader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	/* After linking one technically should call `glDetachShader`
	 * but technically its not necessary.
	 */

	// this deletes the intermediate data
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

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
	/* Create the indices for the rectangle and set the data inside the buffer.
	 * We draw two triangles to create one rectangle.
	 * We reuse two indices via the index_buffer, this is why only have 4 positions instead of 6
	 * see: index_buffer_object 
	 */
	float position[] = {-0.5f, -0.5f,
						0.5f, -0.5f,
						0.5f, 0.5f,
						-0.5f, 0.5f};

	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), position, GL_STATIC_DRAW);

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

	/* See how the positions 0 and 2 get reused */
	unsigned int indices[] = {0, 1, 2,
							  2, 3, 0};
	unsigned int index_buffer_object;
	glGenBuffers(1, &index_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	Shader_program_source shader_program_source = parse_shader_file("res/shader/basic.shader");
	unsigned int shader = create_shader(shader_program_source.vertex_source, shader_program_source.fragment_source);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* We want to draw two TRIANGLES to make up a square */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();

	return 0;
}