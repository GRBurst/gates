#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>

/* #include "inputCallback.h" */
#include "Camera.h"
#include "ModelLoader.h"
#include "PerlinNoise.h"
#include "Portal.h"
#include "Shader.h"
#include "Terrain.h"
#include "Texture.h"
#include "Grass.h"
#include "WorleyNoise.h"


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

Camera camera;
int gDrawGrid = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

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
    //glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void resize_callback( GLFWwindow* p, int newWidth, int newHeight )
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

/* void renderHeightmap(float size, float h, double* heights) */
/* { */

/*     int dim = 50; */
/*     for(int i = 0; i < dim-1; i++) */
/*         for(int j = 0; j < dim-1; j++) */
/*         { */
/*             glBegin(GL_TRIANGLE_STRIP); */
/*             glColor3f(heights[dim * i + j], heights[dim * i + j], heights[dim * i + j]); */
/*             glVertex3f(i*dim, heights[dim * i + j]*h, j*size); */
/*             glVertex3f((i+1)*dim, heights[dim * (i+1) + j]*h, j*size); */
/*             glVertex3f(i*dim, heights[dim * i + (j+1)]*h, (j+1)*size); */
/*             glVertex3f((i+1)*dim, heights[dim * (i+1) + (j+1)]*h, (j+1)*size); */
/*             glEnd(); */
/*         } */

/* } */

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
    GLenum err = GL_NO_ERROR;
    err = glGetError();
	if (err != GL_NO_ERROR)
		std::cout << "Fehler GLEWInit(): " << err << std::endl;
	std::cout.flush();
	initOpenGL();
	//Noise noise(noiseDimX, noiseDimY, Noise::PERLIN, 9, 8, 2.0, 3.0);
	//noise.generateNoiseImage();
	//noise.saveToFile("texture.tga");

    Shader shader;

    /* shader.loadShader("../src/shader/triangle.vs", Shader::VERTEX); */
    /* shader.loadShader("../src/shader/triangle.fs", Shader::FRAGMENT); */
    shader.loadShader("../src/shader/main.vs", Shader::VERTEX);
    shader.loadShader("../src/shader/main.fs", Shader::FRAGMENT);

    /* shader.loadShader("../src/shader/terrain.vs", Shader::VERTEX); */
    /* shader.loadShader("../src/shader/terrain.fs", Shader::FRAGMENT); */
    GLint prog = shader.linkShaders();

    // Portal
    Portal portal(prog);
    portal.init( &camera );

    // Terrain
    Shader terrainshader;
    terrainshader.loadShader("../src/shader/terrain.vs", Shader::VERTEX);
    terrainshader.loadShader("../src/shader/terrain.fs", Shader::FRAGMENT);
    GLint terrainprog = terrainshader.linkShaders();

    int noiseDimX = 256;
    int noiseDimY = 256;
    WorleyNoise wNoise(noiseDimY, noiseDimY);
    wNoise.init();
    wNoise.generateNoiseImage();
    wNoise.saveToFile("wnoise.tga");

    /* Terrain terrainTerrain(terrainprog, noiseDimX, noiseDimY, noiseDimX, noiseDimY, Noise::PERLIN, 9, 8, 2.0, 3.0); */
    /* terrain.setNoiseValues(wNoise.getNoiseValues()); */
    
    int seed = 9;
    int octaves = 8;
    double frequency = 4.0;
    double amplitude = 1.0;
    PerlinNoise noise;
    noise.setParams(noiseDimX, noiseDimY, seed);
    noise.setOctavesFreqAmp(octaves, frequency, amplitude);
    Terrain terrain(terrainprog, noiseDimX, noiseDimY, &noise);
    terrain.setVPMatrix(camera.getVPMatrix());
    terrain.setInvViewMatrix(camera.getInvViewMatrix());
    terrain.enableNormals();
    terrain.computeTerrain();
    terrain.genHeightMapTexture();
	terrain.saveNoiseToFile();

    terrain.linkHeightMapTexture(prog);
    /* terrain.debug(); */



    //GRASS
    Shader grassshader;
    grassshader.loadShader("../src/shader/grass.vs", Shader::VERTEX);
    grassshader.loadShader("../src/shader/grass.gs", Shader::GEOMETRY);
    grassshader.loadShader("../src/shader/grass.fs", Shader::FRAGMENT);
    GLint grassprog = grassshader.linkShaders();

    Grass grass;
//
    grass.setShaderProgram(grassprog);
//    GLfloat billboard[] = {
//        		1.0f, 2.0f, 0.0f,
//				0.1f, 2.0f, -2.0f,
//				0.3f, 2.0f, -1.5f,
//				-0.3f, 2.76f, -1.0f,
//				2.0f,1.0f,2.0f,0.6f,-1.0f,-1.0f
//
//        };

    //grass.generatePositionsFromTexture(noise.getTextureDataF(), noise.getWidth(), noise.getHeight(), 0.2f, 0.7f);

    grass.setTerrainVao(terrain.getVAO(), terrain.getTotalIndices());
    grass.setBuffers();

    grass.setUniforms();
    grass.loadTexture();
    //END GRASS
    //Outsource


    ModelLoader model("../objects/flat_torus.obj", prog);
	model.loadFile();

	model.setBuffers();
	model.setStandardUniformLocations();
	glm::vec3 trans(1.0, 0.0, -1.0);
	model.translate(trans);

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeCallback( window, resize_callback );

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, 0, 0);

    oldTime = glfwGetTime();
    //Noise Test



	double xpos, ypos;
    //model.setView(camera.getViewMatrix());

    while(!(glfwWindowShouldClose(window)))
    {

    	loops = 0;
    	//Update game logic
    	//newTime = std::chrono::steady_clock::now();
    	newTime = glfwGetTime();
    	while (oldTime < newTime && loops < MAX_FRAMESKIP){
    		//here update game   glfwPollEvents();

    	   	glfwGetCursorPos(window, &xpos, &ypos);
    	    glfwPollEvents();
    		/* camera.moveCamera(xpos, ypos, window); */
    		newTime += frameTime;
    		/* camera.setDeltaTime(frameTime); */
    		loops++;
    		//std::cout << "Pos(x, y): " << camera.getCamPos().x <<", " << camera.getCamPos().y << std::endl;
    	}
     	//update Frame
//    	glUseProgram(prog);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        /* std::cout << "view: " << camera.getPos().x << "< " << camera.getPos().y << "< " << camera.getPos().z << std::endl; */
        //neu

        // Portal
        /* portal.drawPortal(camera); */
        portal.drawPortal();

        // sphere
        model.setProjection(camera.getProjectionMatrix());
        model.setView(camera.getViewMatrix());
        model.draw();

        // terrain
        terrain.setVPMatrix(camera.getVPMatrix());
        terrain.setInvViewMatrix(camera.getInvViewMatrix());
        terrain.setGrid(gDrawGrid);
        terrain.draw();
        /* renderHeightmap(0.1, 10 , noise.getTextureData()); */

        //processInput
        //GRASS

 //        err = glGetError();
 //		if (err != GL_NO_ERROR)
 //			std::cout << "Fehler: " << err << std::endl;
         grass.setCameraPosRef(camera.getPos());
         grass.setViewAndProjectionMatrix(camera.getViewMatrix(), camera.getProjectionMatrix());
         grass.draw();

 		//END GRASS


        glfwSwapBuffers( window );
        glFlush();

    }
    // Cleanup VBO
//	glDeleteBuffers(1, &VBO);
//	glDeleteVertexArrays(1, &VAO);

	glDeleteProgram(prog);
	glDeleteProgram(grassprog);
	glDeleteProgram(terrainprog);


}



/* void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode, const Camera &camera, float deltaTime) */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    switch(key)
    {
        case GLFW_KEY_ESCAPE :
            if(action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_Q :
            if(action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_W :
            if(action == GLFW_PRESS) camera.moveForward( float(frameTime) );
            break;
        case GLFW_KEY_S :
            if(action == GLFW_PRESS) camera.moveBack( float(frameTime) );
            break;
        case GLFW_KEY_A :
            if(action == GLFW_PRESS) camera.moveLeft( float(frameTime) );
            break;
        case GLFW_KEY_D :
            if(action == GLFW_PRESS) camera.moveRight( float(frameTime) );
            break;
        case GLFW_KEY_I :
            if(action == GLFW_PRESS) camera.setCamSpeed( 10.0 );
            break;
        case GLFW_KEY_K :
            if(action == GLFW_PRESS) camera.setCamSpeed( 0.1 );
            break;
        case GLFW_KEY_G :
            if(action == GLFW_PRESS) gDrawGrid = (gDrawGrid + 1) % 3;
            break;
        default: std::cout << "Key has no function!" << std::endl;
            break;
    }
        //mRight * (float)mDeltaTime * mCamSpeed
}

/* void mouse_callback(GLFWwindow* window, double xpos, double ypos, const Camera &camera) */
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    int width, height;

    glfwGetWindowSize(window, &width, &height);
    glfwSetCursorPos(window, width/2.0, height/2.0);

    camera.processMouse(float( width/2.0 - xpos ), float( height/2.0 - ypos ));

}

/* void scroll_callback(GLFWwindow* window, double xoffset, double yoffset, const Camera &camera) */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // TODO
}
