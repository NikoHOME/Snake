#define GLFW_DLL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


//stolen Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//stolen Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";


int main()
{
	glfwInit();

	//tell glfw what versions we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	
	//create window and select it
	GLFWwindow* window = glfwCreateWindow(800,600,"snake",NULL,NULL);
	if(window == NULL) {std::cout<<"Failed to create window\n"; glfwTerminate(); return -1;}
	glfwMakeContextCurrent(window);

	//load glad
	gladLoadGL();

	//tell it the render dimensions
	glViewport(0,0,800,600);

	//create all shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
	glCompileShader(fragmentShader);


	//create shader program
	GLuint shaderProgram = glCreateProgram();
	//attach shader
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);
	//delete unneeded shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	GLfloat vertices[] = 
	{
		-1.0f,	-1.0f,	0.0f,
		1.0f,	1.0f,	1.0f,
		-1.0f,	1.0f,	0.0f
	};


	//create vertex buffer and vertex array
	GLuint VAO,VBO;
	//generate VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1,&VBO);
	//bind buffer and vertex array
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	//aquire the vertices data
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	//specify the data fromat
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//enable the VAO index 0
	glEnableVertexAttribArray(0);
	//bind to 0 to not ovverwite it
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);


	//set colour to colour buffer
	glClearColor(0.00f,0.00f,0.00f,1.00f);
	//overwrite buffer with colour buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//swap buffers
	glfwSwapBuffers(window);

	while(!glfwWindowShouldClose(window))
	{

		//set colour to colour buffer
		glClearColor(0.00f,0.00f,0.00f,1.00f);
		//overwrite buffer with colour buffer
		glClear(GL_COLOR_BUFFER_BIT);
		//use shaders
		glUseProgram(shaderProgram);
		//bind the array with vertices
		glBindVertexArray(VAO);
		//draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//swap buffers
		glfwSwapBuffers(window);
		//manage glfw events
		glfwPollEvents();
	}
	
	glDeleteBuffers(1,&VBO);
	glDeleteVertexArrays(1,&VAO);
	glDeleteProgram(shaderProgram);


	glfwDestroyWindow(window);
	glfwTerminate();

	
	
	
	
	
	
	return 0;
}