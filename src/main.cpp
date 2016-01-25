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
#include "Skydome.h"
#include "Clouds.h"


using namespace glm;

unsigned int wWidth = 1024, wHeight = 768;
GLFWwindow* window;

const int UPDATES_PER_SECOND = 60;
const int MAX_FRAMESKIP = 10;
const double frameTime = 1.0 / UPDATES_PER_SECOND;

Camera camera;
int gDrawGrid = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double mouseXPosition, double mouseYPosition);
    bool portalIntersection(Camera *const camera, Terrain*& pActiveTerrain, Terrain*& pInactiveTerrain, Portal*& pActivePortal, Portal*& pInactivePortal, const GLint& terrainShaderProgram, const GLint& portalprog);

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
    glEnable(GL_CULL_FACE);
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

int main()
{

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
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    GLenum err = GL_NO_ERROR;
    err = glGetError();
    if (err != GL_NO_ERROR)
        std::cout << "Fehler GLEWInit(): " << err << std::endl;
    std::cout.flush();
    initOpenGL();

    // Input initialization
    glfwSetKeyCallback( window, key_callback );
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeCallback( window, resize_callback );

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, 0, 0);
    double mouseXPosition, mouseYPosition;

    /*
     * Begin main initialization phase
     *
     */

    // Default shader
    Shader defaultShader;
    defaultShader.loadShader("../src/shader/main.vs", Shader::VERTEX);
    defaultShader.loadShader("../src/shader/main.fs", Shader::FRAGMENT);
    GLint defaultShaderProgram = defaultShader.linkShaders();

    // Grass shader
    Shader grassShader;
    grassShader.loadShader("../src/shader/grass.vs", Shader::VERTEX);
    grassShader.loadShader("../src/shader/grass.gs", Shader::GEOMETRY);
    grassShader.loadShader("../src/shader/grass.fs", Shader::FRAGMENT);
    GLint grassShaderProgram = grassShader.linkShaders();

    // Terrain
    Shader terrainShader;
    terrainShader.loadShader("../src/shader/terrain.vs", Shader::VERTEX);
    terrainShader.loadShader("../src/shader/terrain.fs", Shader::FRAGMENT);
    GLint terrainShaderProgram = terrainShader.linkShaders();

    // Skydom
    Shader skydomeShader;
    skydomeShader.loadShader("../src/shader/skydome.vs", Shader::VERTEX);
    skydomeShader.loadShader("../src/shader/skydome.fs", Shader::FRAGMENT);
    GLint skydomeShaderProgram = skydomeShader.linkShaders();

    // Cloids
    Shader cloudsShader;
    cloudsShader.loadShader("../src/shader/clouds.vs", Shader::VERTEX);
    cloudsShader.loadShader("../src/shader/clouds.gs", Shader::GEOMETRY);
    cloudsShader.loadShader("../src/shader/clouds.fs", Shader::FRAGMENT);
    GLint cloudsShaderProgram = cloudsShader.linkShaders();
    /*
     * Setup main Pointer
     * Terrain Pointer: pActiveTerrain, pInactiveTerrain
     * Portal Pointer: pActivePortal, pInactivePortal
     *
     */

    Terrain *pActiveTerrain, *pInactiveTerrain;
    Portal *pActivePortal, *pInactivePortal;
    Noise *pActiveNoise, *pInactiveNoise;

    /*
     * Setup initial terrains and noise
     * First terrain uses perlin noise
     * Second uses worley noise
     * Use common parameters at current state of development
     *
     */

    // Common parameters
    int noiseDimX = 256, noiseDimY = 256, noiseDimZ = 64;
    int seed = 42, octaves = 16;
    double frequency = 8.0, amplitude = 4.0;

    // Setup perlin noise
    pActiveNoise = new PerlinNoise();
    pActiveNoise->setParams(noiseDimX, noiseDimY, seed);
    pActiveNoise->setOctavesFreqAmp(octaves, frequency, amplitude);

    // Setup initial terrain
    pActiveTerrain = new Terrain(terrainShaderProgram, noiseDimX, noiseDimY, pActiveNoise);
    pActiveTerrain->setVPMatrix(camera.getVPMatrix());
    pActiveTerrain->setInvViewMatrix(camera.getInvViewMatrix());
    pActiveTerrain->enableNormals();
    pActiveTerrain->computeTerrain();
    pActiveTerrain->genHeightMapTexture();
    pActiveTerrain->saveNoiseToFile("PerlinNoise_Terrain.tga");
    pActiveTerrain->linkHeightMapTexture(terrainShaderProgram);
    pActiveTerrain->linkHeightMapTexture(defaultShaderProgram);

    // Setup worled (cell) noise for second terrain
    pInactiveNoise = new WorleyNoise();
    pInactiveNoise->setParams(noiseDimX, noiseDimY, seed);
    pInactiveNoise->setOctavesFreqAmp(octaves, frequency, amplitude);

    // Setup initial inactive terrain (in portal)
    pInactiveTerrain = new Terrain(terrainShaderProgram, noiseDimX, noiseDimY, pInactiveNoise);
    pInactiveTerrain->setVPMatrix(camera.getVPMatrix());
    pInactiveTerrain->setInvViewMatrix(camera.getInvViewMatrix());
    pInactiveTerrain->enableNormals();
    pInactiveTerrain->computeTerrain();
    pInactiveTerrain->genHeightMapTexture();
    pInactiveTerrain->saveNoiseToFile("WorleyNoise_Terrain.tga");

    /*
     * Setup Portals
     * Each terrain will have one inactive portal (the portal where user came from)
     * and an active portal to teleport to the next terrain
     *
     */

    // Portal
    pActivePortal = new Portal(defaultShaderProgram);
    pActivePortal->init( &camera, pActiveTerrain, pInactiveTerrain );

    pInactivePortal = new Portal(defaultShaderProgram);
    pInactivePortal->init( &camera, pActiveTerrain, pInactiveTerrain );
    pInactivePortal->deaktivate();

    /*
     * Setup further terrain properties like skydome, grass, ...
     *
     */

    // Skydome noise parameters
    int noiseSkyDimX = 256, noiseSkyDimY = 128, noiseSkyDimZ = 64;
    seed = 42, octaves = 2, frequency = 2.0;

    // Setup noise for clouds
    PerlinNoise pNoise3D;
    pNoise3D.setParams(noiseSkyDimX, noiseSkyDimY, noiseSkyDimZ, seed);
    pNoise3D.setOctavesFreqAmp(octaves, frequency, amplitude);
    pNoise3D.generateNoiseImage();

    // Skydome initialization
    Skydome skydome(skydomeShaderProgram, &camera);
    skydome.generateGeometry(noiseDimX / 3, 64, 64);
    skydome.setBuffers();

    // Clouds initialization
    Clouds clouds(cloudsShaderProgram, skydome.getCloudNumber(), skydome.getCloudAttributes(), &camera);
    clouds.setBuffers();
    clouds.loadTexture(pNoise3D.getTextureData(), noiseSkyDimX, noiseSkyDimY, noiseSkyDimZ);
    skydome.setClouds(&clouds);

    // Grass
    Grass grass;
    grass.setShaderProgram(grassShaderProgram);
    grass.setTerrainVao(pActiveTerrain->getVAO(), pActiveTerrain->getTotalIndices());
    grass.setUniforms();
    grass.loadTexture();

    // Our beloved sphere in the center of the terrain :-)
    ModelLoader model("../objects/sphere.obj", defaultShaderProgram);
    model.loadFile();
    model.setBuffers();
    model.setStandardUniformLocations();
    vec3 sphereTranslation = vec3(0.0, 3.0, 0.0);
    model.translate(sphereTranslation);

    // Main (frame) loop
    float oldTime, newTime;
    int loops;
    oldTime = glfwGetTime();
    /* int noises = 0; */
    while(!(glfwWindowShouldClose(window)))
    {
        loops = 0;

        //Update game logic and physics
        newTime = glfwGetTime();
        while (oldTime < newTime && loops < MAX_FRAMESKIP)
        {
            //here update game   glfwPollEvents();

            glfwGetCursorPos(window, &mouseXPosition, &mouseYPosition);
            glfwPollEvents();
            newTime += frameTime;

            loops++;

            /*
             * portalIntersection(&camera, pActiveTerrain, pInactiveTerrain, pActivePortal, pInactivePortal, terrainShaderProgram, defaultShaderProgram); 
             *
             * Check for portal intersection
             *
             */

            float diff = glm::length(glm::vec3(camera.getPosition() - pActivePortal->getPosition()));

            if((diff < 2.5) && pActivePortal->isActive())
            {
                // Do teleportation
                pActivePortal->teleport();
                camera.setPosition(pActivePortal->getPosition2());
                /* camera->setOrientation(); */
                camera.update();

                std::cout << "Generate new noise" << std::endl;
                // Generate new inactive terrain
                int noiseDimX = 256;
                int noiseDimY = 256;
                double seed = 1.0 + 42.0 * pActivePortal->getPosition().x;
                int octaves = static_cast<int>(4.0 * seed) % 17;
                double frequency = mod(seed, 67.0);
                double amplitude = mod(seed, 11.0);
                Noise* pNewNoise = new PerlinNoise();
                pNewNoise->setParams(noiseDimX, noiseDimY, seed);
                pNewNoise->setOctavesFreqAmp(octaves, frequency, amplitude);

                delete pActiveNoise;
                pActiveNoise    = pInactiveNoise;
                pInactiveNoise  = pNewNoise;

                std::cout << "Generate new terrain" << std::endl;
                Terrain* pNewTerrain = new Terrain(terrainShaderProgram, noiseDimX, noiseDimY, pInactiveNoise);
                pNewTerrain->setVPMatrix(camera.getVPMatrix());
                pNewTerrain->setInvViewMatrix(camera.getInvViewMatrix());
                pNewTerrain->enableNormals();
                pNewTerrain->computeTerrain();
                pNewTerrain->genHeightMapTexture();
                pNewTerrain->saveNoiseToFile("newTerrain_noise.tga");
                pNewTerrain->linkHeightMapTexture(terrainShaderProgram);
                pNewTerrain->linkHeightMapTexture(defaultShaderProgram);
                pNewTerrain->draw();

                // swap terrains
                delete pActiveTerrain;
                pActiveTerrain      = pInactiveTerrain;
                pInactiveTerrain    = pNewTerrain;


                std::cout << "Generate new portal" << std::endl;
                // Setup new portal
                Portal* pNewPortal = new Portal(defaultShaderProgram);
                pNewPortal->init( &camera, pActiveTerrain, pInactiveTerrain );

                // swap portals
                delete pInactivePortal;
                pInactivePortal = pActivePortal;
                pActivePortal   = pNewPortal;
            }
        }

        //update Frame

        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);

        // Recalculate camera matrices
        camera.update();






        // Enable stencil and automatically draw pattern
        pActivePortal->enableStencil();





        // Draw around pattern (main scene, around portal)
        pActivePortal->renderOutside();


        // Portal
        pInactivePortal->drawPortal();

        // Sphere
        model.setProjection(camera.getProjectionMatrix());
        model.setView(camera.getViewMatrix());
        model.draw();

        // Active terrain
        pActiveTerrain->setVPMatrix(camera.getVPMatrix());
        pActiveTerrain->setInvViewMatrix(camera.getInvViewMatrix());
        pActiveTerrain->setGrid(gDrawGrid);
        pActiveTerrain->draw();

        // Grass
        grass.setViewAndProjectionMatrix(camera.getViewMatrix(), camera.getProjectionMatrix());
        grass.draw();

        // Skydome
        skydome.draw();




        // Draw in stencil pattern (in portal)
        pActivePortal->renderInside();





        // Sphere
        model.setProjection(camera.getProjectionMatrix());
        model.setView(camera.getViewMatrix());
        model.draw();

        // Inactive or next terrain
        pInactiveTerrain->setVPMatrix(camera.getVPMatrix());
        pInactiveTerrain->setInvViewMatrix(camera.getInvViewMatrix());
        pInactiveTerrain->setGrid(gDrawGrid);
        pInactiveTerrain->draw();

        // Grass
        grass.setViewAndProjectionMatrix(camera.getViewMatrix(), camera.getProjectionMatrix());
        grass.draw();

        // Skydome
        skydome.draw();





        // Disable stencil test
        pActivePortal->disableStencil();

        // Do post rendering stuff (maybe shadowmaps etc. if enough spare time)



        glfwSwapBuffers( window );
        glFlush();

    }

    /*
     * Cleaup phase
     */


    // Pointers
    delete pActiveTerrain;
    delete pInactiveTerrain;
    delete pActivePortal;
    delete pInactivePortal;
    delete pActiveNoise;
    delete pInactiveNoise;

    // Programs
    glDeleteProgram(defaultShaderProgram);
    glDeleteProgram(grassShaderProgram);
    glDeleteProgram(terrainShaderProgram);
    glDeleteProgram(skydomeShaderProgram);
    glDeleteProgram(cloudsShaderProgram);

}

    bool portalIntersection(Camera *const camera, Terrain*& pActiveTerrain, Terrain*& pInactiveTerrain, Portal*& pActivePortal, Portal*& pInactivePortal, const GLint& terrainShaderProgram, const GLint& portalProg)
{

    float diff = glm::length(glm::vec3(camera->getPosition() - pActivePortal->getPosition()));

    if((diff < 2.5) && pActivePortal->isActive())
    {
        // Do teleportation
        pActivePortal->teleport();
        camera->setPosition(pActivePortal->getPosition2());
        /* camera->setOrientation(); */
        camera->update();

        // Generate new inactive terrain
        int noiseDimX = 256;
        int noiseDimY = 256;
        int seed = 42 * pActivePortal->getPosition().x;
        int octaves = 4;
        double frequency = 54.0;
        double amplitude = 2.0;
        PerlinNoise pNoise;
        pNoise.setParams(noiseDimX, noiseDimY, seed);
        pNoise.setOctavesFreqAmp(octaves, frequency, amplitude);

        Terrain* pNewTerrain = new Terrain(terrainShaderProgram, noiseDimX, noiseDimY, &pNoise);
        pNewTerrain->setVPMatrix(camera->getVPMatrix());
        pNewTerrain->setInvViewMatrix(camera->getInvViewMatrix());
        pNewTerrain->enableNormals();
        pNewTerrain->computeTerrain();
        pNewTerrain->genHeightMapTexture();
        pNewTerrain->saveNoiseToFile("PerlinNoise_Terrain2.tga");
        pNewTerrain->linkHeightMapTexture(terrainShaderProgram);
        pNewTerrain->linkHeightMapTexture(portalProg);
        pNewTerrain->draw();

        // swap terrains
        delete pActiveTerrain;
        pActiveTerrain      = pInactiveTerrain;
        pInactiveTerrain    = pNewTerrain;


        // Setup new portal
        Portal* pNewPortal = new Portal(portalProg);
        pNewPortal->init( camera, pActiveTerrain, pInactiveTerrain );

        // swap portals
        delete pInactivePortal;
        pInactivePortal = pActivePortal;
        pActivePortal   = pNewPortal;

        return true;
    }

    return false;

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
            if(action == GLFW_REPEAT) camera.moveForward( float(frameTime) );
            break;
        case GLFW_KEY_S :
            if(action == GLFW_REPEAT) camera.moveBack( float(frameTime) );
            break;
        case GLFW_KEY_A :
            if(action == GLFW_REPEAT) camera.moveLeft( float(frameTime) );
            break;
        case GLFW_KEY_D :
            if(action == GLFW_REPEAT) camera.moveRight( float(frameTime) );
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
        case GLFW_KEY_C :
            if(action == GLFW_PRESS)
            {
                std::cout << std::endl << "Camera coords: x = " << camera.getPosition().x << ", y = " << camera.getPosition().y << ", z = " << camera.getPosition().z << std::endl;
            }
            break;

        default: std::cout << "Key has no function!" << std::endl;
            break;
    }
        //mRight * (float)mDeltaTime * mCamSpeed
}

/* void mouse_callback(GLFWwindow* window, double mouseXPosition, double mouseYPosition, const Camera &camera) */
void mouse_callback(GLFWwindow* window, double mouseXPosition, double mouseYPosition)
{
    int width, height;

    glfwGetWindowSize(window, &width, &height);
    glfwSetCursorPos(window, width/2.0, height/2.0);

    camera.processMouse(float( width/2.0 - mouseXPosition ), float( height/2.0 - mouseYPosition ));

}

/* void scroll_callback(GLFWwindow* window, double xoffset, double yoffset, const Camera &camera) */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // TODO
}
