#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>
#include "Shader.h"
#include "keyCallback.h"
#include "Noise.h"
#include "camera.h"
#include "ModelLoader.h"


using namespace glm;

unsigned int wWidth = 1024, wHeight = 768;
GLFWwindow* window;

const int UPDATES_PER_SECOND = 60;
const int MAX_FRAMESKIP = 10;
double frameTime = 1.0 / UPDATES_PER_SECOND;
//std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> oldTime;
//std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> newTime = std::chrono::steady_clock::now();;
float oldTime, newTime;
int loops;

void debugCallback(GLenum source, GLenum type, GLuint id,
                   GLenum severity, GLsizei length,
                   const GLchar *message, const void *userParam)
{
    std::cout << "Note: ";
     if (source == GL_DEBUG_SOURCE_API_ARB)
            std::cout << "OpenGL";
     else if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
            std::cout << "your OS";
     else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
            std::cout << "the Shader Compiler";
     else if (source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)
            std::cout << "a third party component";
     else if (source == GL_DEBUG_SOURCE_APPLICATION_ARB)
            std::cout << "your application";
     else if (source == GL_DEBUG_SOURCE_OTHER_ARB)
            std::cout << "someone";

     std::cout << " reported a problem - it's a";
     if (type == GL_DEBUG_TYPE_ERROR_ARB)
            std::cout << "n error";
     else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)
            std::cout << " deprecated behavior";
     else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
            std::cout << "n undefined behavior";
     else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB)
            std::cout << " portability issue";
     else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB)
            std::cout << " performance issue";
     else if(type == GL_DEBUG_TYPE_OTHER_ARB)
            std::cout << " something";

     std::cout << std::endl;
     std::cout << "The message was: " << message << std::endl << std::endl;
}

bool createWindow()
{
    // OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //No old OpenGL 
   // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Make MacOS happy


    // 4x antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // request an OpenGL debug context:
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, true );

    // define whether the window can get resized:
    glfwWindowHint( GLFW_RESIZABLE, true );


    // Open a window and create its OpenGL context
    window = glfwCreateWindow( wWidth, wHeight, "G.A.T.E.S.", NULL, NULL);
    if( window == NULL )
    {
        fprintf( stderr, "Failed to open GLFW window.\n" );
        return false;
    }

    glfwMakeContextCurrent(window); // Initialize GLEW

    return true;

}

void initOpenGL() 
{
	/* int glInit = ogl_LoadFunctionsForDebug( GL_TRUE, GL_TRUE ); */
	
	/* if (glInit != ogl_LOAD_SUCCEEDED) { */
	/* 	cerr << "could not initialize OpenGL" << endl; */
	/* 	exit(1); */
	/* } */
	
	/* if (ogl_ext_KHR_debug) { */
	/* 	glDebugMessageCallback( debugCallback, NULL ); */
    /* } */

    glViewport(0, 0, wWidth, wHeight);
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void resizeCallback( GLFWwindow* p, int newWidth, int newHeight )
{
    wWidth  = newWidth;
    wHeight = newHeight;

    //resizeCallback( wWidth, wHeight );glViewport( 0, 0, newWidth, newHeight );
    glViewport( 0, 0, newWidth, newHeight );

}

void updateGame(){

}

void displayFrame(){

}

int main(){

    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit(-1);
    }

    if(!createWindow())
    {
        fprintf( stderr, "Failed to create window\n" );
        glfwTerminate();
        exit(-1);
    }

    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    initOpenGL();
    GLenum err = GL_NO_ERROR;
    err = glGetError();
    Shader shader;
    Camera camera;

    /* shader.loadShader("../src/shader/triangle.vs", Shader::VERTEX); */
    /* shader.loadShader("../src/shader/triangle.fs", Shader::FRAGMENT); */
    shader.loadShader("../src/shader/main.vs", Shader::VERTEX);
    shader.loadShader("../src/shader/main.fs", Shader::FRAGMENT);
    GLint prog = shader.linkShaders();

    //Outsource

    ModelLoader *model = new ModelLoader("../objects/sphere.obj", prog);
	model->loadFile();
	model->setProjection(camera.getProjectionMatrix());
	model->setBuffers();
	model->setStandardUniformLocations();
	glm::vec3 trans(1.0, 0.0, -1.0);
	model->translate(trans);

//    GLuint posAttrib = glGetAttribLocation(prog, "vPosition");
//    GLuint VAO;
//
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    GLfloat vertexBuffer[] = {
//    		-1.0f, -1.0f, -0.5f,
//			1.0f, -1.0f, -0.6f,
//   		    0.0f,  1.0f, -1.0f,
//    };
//
//    GLuint VBO;
//    glGenBuffers(1, &VBO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
//
//    glEnableVertexAttribArray(posAttrib);
//    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
//    glBindVertexArray(0);
    //set Uniforms
    //GLint mvpMat = glGetUniformLocation(prog, "ModelViewProjectionMatrix");
    // Ensure we can capture the escape key being pressed below
    glfwSetKeyCallback( window, key_callback );
    glfwSetWindowSizeCallback( window, resizeCallback );

    oldTime = glfwGetTime();
    //Noise Test
    Noise noise(500, 500, Noise::PERLIN, 1231, 8, 2.0, 1.0);
    noise.generateNoiseImage();
    noise.saveToFile("texture.tga");


    //model->setView(camera.getViewMatrix());

    while(!(glfwWindowShouldClose(window)))
    {
    	loops = 0;
    	//Update game logic
    	//newTime = std::chrono::steady_clock::now();
    	newTime = glfwGetTime();
    	while (oldTime < newTime && loops < MAX_FRAMESKIP){
    		//here update game   glfwPollEvents();
    	   	double xpos, ypos;
    	   	glfwGetCursorPos(window, &xpos, &ypos);

    		camera.moveCamera(xpos, ypos, window);
    		newTime += frameTime;
    		camera.setDeltaTime(frameTime);
    		loops++;
    		//std::cout << "Pos(x, y): " << camera.getCamPos().x <<", " << camera.getCamPos().y << std::endl;
    	}
    	//update Frame
//    	glUseProgram(prog);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);
     //   std::cout << "view: " << camera.getCamPos().x << "< " << camera.getCamPos().y << "< " << camera.getCamPos().z << std::endl;
        //neu
        model->setProjection(camera.getProjectionMatrix());
        model->setView(camera.getViewMatrix());

        model->draw();
        err = glGetError();
        	        if (err != GL_NO_ERROR)
        	        	std::cout << "Fehler: " << err << std::endl;
        //processInput
        glfwPollEvents();

        glfwSwapBuffers( window );


    }
    // Cleanup VBO
//	glDeleteBuffers(1, &VBO);
//	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(prog);

}
