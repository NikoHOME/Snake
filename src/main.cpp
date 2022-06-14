#define GLFW_DLL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <deque>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <deque>
#include <vector>

#define checktime sc::duration_cast<sc::milliseconds>(sc::system_clock::now().time_since_epoch()).count()

namespace sc=std::chrono;

//stolen Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//stolen Fragment Shader source code
const char* fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.1f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.2f, 0.6f, 0.02f, 1.0f);\n"
"}\n\0";

//variables

auto currentTime = checktime;
int tickspersecond = 1000/5,boostLimit=1000/8;
int dir=0;

    //Up 0
    //Down 1
    //Left 2
    //Right 3

bool gameOver = false;
int posx,posy,fruitx,fruity;
bool tab[40][30];
    
std::deque <std::pair <int,int>> que;


void runtime()
{

	
       //controls
        if(GetKeyState('W') < 0 && dir!=1) dir=0;
        if(GetKeyState('S') < 0 && dir!=0) dir=1;
        if(GetKeyState('A') < 0 && dir!=3) dir=2;
        if(GetKeyState('D') < 0 && dir!=2) dir=3;
        if(GetKeyState(27)<0 ) return;

        


        if(checktime>=currentTime+tickspersecond) 
        {
            if((posx==0 && dir==2) || (posx==39 && dir==3) || (posy==0 && dir==1) || (posy==29 && dir==0))
            {
                gameOver=true;
                return;
            }
            que.push_back({posx,posy});
            //clear the screen
            for(int i=0;i<40;++i) for(int j=0;j<30;++j) tab[i][j]=false;
                
            //copy segments
            for(auto j:que) tab[j.first][j.second]=true;

            switch(dir)
            {
                case 0:
                    //std::cout<<"HE WENT UP\n";
                    ++posy;
                break;
                case 1:
                    //std::cout<<"HE WENT DOWN\n";
                    --posy;
                break;
                case 2:
                    //std::cout<<"HE WENT LEFT\n";
                    --posx;
                break;
                case 3:
                    //std::cout<<"HE WENT RIGHT\n";
                    ++posx;
                break;
            }

            if(tab[posx][posy]) 
            {
                gameOver=true;
                return;
            }
            else tab[posx][posy]=true;
            //remove the last snake segment if fruit not eaten
            if(posx==fruitx && posy==fruity)
            {
                while(tab[fruitx][fruity])
                {
                    fruitx=rand()%40;
                    if(!tab[fruitx][fruity]) break;
                    fruity=rand()%30;
                }
            } 
            else que.pop_front();
            //draw the snake
            for(int i=29;i>=0;--i) 
            {
                for(int j=0;j<40;++j) 
                {
                    if(tab[j][i]) std::cout<<"#";
                    else if(j==fruitx and i==fruity)
                    {
                        std::cout<<"@";
                    }
                    else std::cout<<".";
                }
                std::cout<<"\n";
            }
            currentTime+=tickspersecond;
            //std::cout<<"TICK\n";
        }
	return;
}

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
	glShaderSource(fragmentShader,1,&fragmentShaderSource1,NULL);
	glCompileShader(fragmentShader);


	//create shader program
	GLuint shaderProgram1 = glCreateProgram();
	
	//attach shader
	glAttachShader(shaderProgram1,vertexShader);
	glAttachShader(shaderProgram1,fragmentShader);
	glLinkProgram(shaderProgram1);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragmentShaderSource2,NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram2 = glCreateProgram();

	glAttachShader(shaderProgram2,vertexShader);
	glAttachShader(shaderProgram2,fragmentShader);
	glLinkProgram(shaderProgram2);
	//delete unneeded shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	struct nodes{
		GLfloat x1,y1,z1,
				x2,y2,z2,
				x3,y3,z3,
				x4,y4,z4;

	};
	struct indices{

		unsigned int i1,i2,i3,i4,i5,i6;

	};

	nodes vertices[] = 
	{
		{-0.9f,	0.45f,	0.0f,
		0.9f,	0.45f,	0.0f,
		-0.9f,	-0.9f,	0.0f,
		0.9f,	-0.9f,	0.0f}
	};

	std::vector <nodes> snakeVertices;
	std::vector <indices> snakeIndices;
	std::vector <nodes> fruitVertices;
	fruitVertices.push_back
	({
			-0.585f, -0.855f, 0.0f,
			-0.54f, -0.855f, 0.0f,
			-0.585f, -0.9f, 0.0f,
			-0.54f, -0.9f, 0.0f
	});

	//create vertex buffer and vertex array
	GLuint VAO[3],VBO[3],EBO; //0-background 1-snake 2-fruit
	//generate VAO and VBO
	glGenVertexArrays(1, &VAO[0]);
	glGenBuffers(1,&VBO[0]);
	//bind buffer and vertex array
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);
	//aquire the vertices data
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	//specify the data format
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0, 0);
	//enable the VAO index 0
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &VAO[1]);
	glGenBuffers(1,&VBO[1]);
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);
	glBufferData(GL_ARRAY_BUFFER,1201*3*sizeof(int),nullptr,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1201*3*sizeof(int), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	glGenVertexArrays(1, &VAO[2]);
	glGenBuffers(1,&VBO[2]);
	//bind buffer and vertex array

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER,VBO[2]);
	//aquire the vertices data
	glBufferData(GL_ARRAY_BUFFER,3*4*sizeof(GL_FLOAT),nullptr,GL_DYNAMIC_DRAW);
	//specify the data format
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//enable the VAO index 0
	glEnableVertexAttribArray(0);

	//bind to 0 to reset
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	

	//set colour to colour buffer
	glClearColor(0.00f,0.00f,0.00f,1.00f);
	//overwrite buffer with colour buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//swap buffers
	glfwSwapBuffers(window);

	//Game Setup
/*
	auto currentTime = checktime;
    std::cout<<"START\n";
    int tickspersecond = 1000/5;
    int dir=0;

    //Up 0
    //Down 1
    //Left 2
    //Right 3
    srand(currentTime);
    bool gameOver = false;
    int posx=19,posy=14,fruitx=rand()%40,fruity=rand()%30;
    bool tab[40][30];
    
    std::deque <std::pair <int,int>> que;
*/
	srand(currentTime);
	posx=19;
	posy=14;
	fruitx=rand()%40;
	fruity=rand()%30;
	float increment = 0.045f;

    //declare starting snake
    que.push_back({19,8});
    que.push_back({19,9});
    que.push_back({19,10});
    que.push_back({19,11});
    que.push_back({19,12});
    que.push_back({19,13});
	
	std::cout<<"START\n";
	bool isStarted = false;
	while(!glfwWindowShouldClose(window))
	{

		//set colour to colour buffer
		glClearColor(0.00f,0.00f,0.00f,1.00f);
		//overwrite buffer with colour buffer
		glClear(GL_COLOR_BUFFER_BIT);
		//use shaders
		glUseProgram(shaderProgram1);
		//bind the array with vertices
		glBindVertexArray(VAO[0]);
		//draw the triangles into rectangle
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//the game logic
		if(!gameOver && isStarted) runtime();
		else if(gameOver) 
		{
			std::cout<<"GAME OVER\n";
			break;
		}
		else
		{
			if(GetKeyState('W') < 0 || GetKeyState('S') < 0 || GetKeyState('A') < 0 || GetKeyState('D') < 0) isStarted=true;
		}
		snakeVertices.clear();
		snakeIndices.clear();
		fruitVertices.clear();
		float posX,posY;
		for(int i=29;i>=0;--i) 
            {
                for(int j=0;j<40;++j) 
                {
                    if(tab[j][i]) 
					{
						std::cout<<"#"; 			// i=y j=x grid:40x30 GL:1.8x1.35 increment:0.045
						posX=j*increment;
                		posY=i*increment;
						snakeVertices.push_back
						({
							-0.9f+posX, -0.9f+posY, 0.0f, 
                        	-0.9f+posX, -0.855f+posY, 0.0f,  
                        	-0.855f+posX, -0.855f+posY, 0.0f,
                        	-0.855f+posX, -0.9f+posY, 0.0f
						});
						
						
					}
					else if(j==fruitx and i==fruity)		// starting point -0.9 -0.9
                    {										// 
                        std::cout<<"@";	
						posX=j*increment;
                		posY=i*increment;
						fruitVertices.push_back
						({
							-0.9f+posX, -0.9f+posY, 0.0f, 
                        	-0.9f+posX, -0.855f+posY, 0.0f,  
                        	-0.855f+posX, -0.855f+posY, 0.0f,
                        	-0.855f+posX, -0.9f+posY, 0.0f
						});					//
                    }
                    else std::cout<<".";
                }
                std::cout<<"\n";
            }

		for(unsigned int i=0;i<snakeVertices.size()*4;i+=4)
            {
                snakeIndices.push_back({
                
                    i,i+1,i+3,
                    i+1,i+3,i+2
				});
                
            }
		
		glBindVertexArray(VAO[1]);
		glUseProgram(shaderProgram2);
		glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);
		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(GLfloat)*snakeVertices.size()*12,&snakeVertices[0]);
		glBindBuffer(GL_ARRAY_BUFFER,EBO);	
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,sizeof(unsigned int)*snakeIndices.size()*6,&snakeIndices[0]);
		glDrawElements(GL_TRIANGLES, snakeIndices.size()*6, GL_UNSIGNED_INT,0);
		//swap buffers

		glBindVertexArray(VAO[2]);
		glBindBuffer(GL_ARRAY_BUFFER,VBO[2]);
		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(GLfloat)*4*3,&fruitVertices[0]);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		glfwSwapBuffers(window);
		glBindVertexArray(0);
		//manage glfw events
		glfwPollEvents();
		
	}
	
	for(auto j:snakeVertices)
	{
		std::cout<<j.x1<<" "<<j.y1<<"\n"<<j.x2<<" "<<j.y2<<"\n"<<j.x3<<" "<<j.y3<<"\n"<<j.x4<<" "<<j.y4<<"\n\n";
	}	

	for(auto j:snakeIndices)
	{
		std::cout<<j.i1<<" "<<j.i2<<" "<<j.i3<<"\n"<<j.i4<<" "<<j.i5<<" "<<j.i6<<"\n\n";
	}	

	


	glDeleteBuffers(2,VBO);
	glDeleteVertexArrays(2,VAO);
	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);



	glfwDestroyWindow(window);
	glfwTerminate();

	
	
	
	
	
	
	return 0;
}