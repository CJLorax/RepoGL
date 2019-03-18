//============================================================================
// Name        : SDL_OpenGL.cpp
// Author      : Casey Jones
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#if defined(__APPLE__)


	#include </usr/local/include/glm/glm.hpp>
	#include </usr/local/include/GL/glew.h>

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_opengl.h>
//	#include <SDL2_image/SDL_image.h>
//	#include <SDL2_net/SDL_net.h>
//	#include <SDL2_mixer/SDL_mixer.h>
//	#include <SDL2_ttf/SDL_ttf.h>

	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>

	#include <unistd.h>

#endif

#if defined(__linux__)


	#include <glm/glm.hpp>
	#include <GL/glew.h>

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_opengl.h>
//	#include <SDL2_image/SDL_image.h>
//	#include <SDL2_net/SDL_net.h>
//	#include <SDL2_mixer/SDL_mixer.h>
//	#include <SDL2_ttf/SDL_ttf.h>

	#include <GL/gl.h>
	#include <GL/glu.h>

	#include <unistd.h>

#endif

#include <iostream>
using namespace std;

// Our SDL_Window ( just like with SDL2 without OpenGL)
SDL_Window *mainWindow;

// Our opengl context handle
SDL_GLContext mainContext;






const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main() {



	// Initialize SDL's subsystems
	    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	    {
	        std::cout << "Failed to init SDL\n";
	        return false;
	    }

	    // Create our window centered at 512x512 resolution
	    mainWindow = SDL_CreateWindow("OpenGL Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	1024, 768, SDL_WINDOW_OPENGL);

	    // Check that everything worked out okay
	    if (!mainWindow)
	    {
	        std::cout << "Unable to create window\n";
	        return false;
	    }


	    // Set our OpenGL version.
	    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	    // 3.2 is part of the modern versions of OpenGL, but most video cards would be able to run it
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	    // Turn on double buffering with a 24bit Z buffer.
	    // You may need to change this to 16 or 32 for your system
	    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	    // Create our opengl context and attach it to our window
	    mainContext = SDL_GL_CreateContext(mainWindow);

	    // This makes our buffer swap syncronized with the monitor's vertical refresh
	    SDL_GL_SetSwapInterval(1);

	    // turn GLEW experimental on
	    glewExperimental= GL_TRUE;

	    // check for GLEW init()
	    GLenum err=glewInit();
	    if(err!=GLEW_OK) {
	      // Problem: glewInit failed, something is seriously wrong.
	      cout << "glewInit failed: " << glewGetErrorString(err) << endl;
	      exit(1);
	    }

	    //SDL event to handle input
		SDL_Event event;

	    bool quit = false;







	    // build and compile our shader program
		// ------------------------------------
		// vertex shader
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// fragment shader
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// link shaders
		int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, // left
			 0.5f, -0.5f, 0.0f, // right
			 0.0f,  0.5f, 0.0f  // top
		};

		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);






	    while(!quit){


			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					quit = true;

				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						quit = true;
						break;
					default:
						break;
					}
				}
			}


	        // render
	        // ------
	        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	        glClear(GL_COLOR_BUFFER_BIT);
	        //SDL_GL_SwapWindow(mainWindow);

	        // draw our first triangle
	        glUseProgram(shaderProgram);
	        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	        glDrawArrays(GL_TRIANGLES, 0, 3);
	        // glBindVertexArray(0); // no need to unbind it every time

	        SDL_GL_SwapWindow(mainWindow);


	    }

	    // optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		// Delete our OpengL context
		SDL_GL_DeleteContext(mainContext);

		// Destroy our window
		SDL_DestroyWindow(mainWindow);

		// Shutdown SDL 2
		SDL_Quit();

	return 0;
}
