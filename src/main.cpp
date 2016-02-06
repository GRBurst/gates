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
#include "ImprovedPerlinNoise.h"
#include "SimplexNoise.h"


using namespace glm;

unsigned int wWidth = 1024, wHeight = 768;
GLFWwindow* window;

const int UPDATES_PER_SECOND = 60;
const int MAX_FRAMESKIP = 10;
const double frameTime = 1.0 / UPDATES_PER_SECOND;

Camera camera;
Terrain *pActiveTerrain;
double lastMouseXPosition, lastMouseYPosition;
static int gDrawGrid = 0;
static unsigned int gEditMode = 2;

glm::vec3 getWorldRayFromCursor(const double& screenPosX, const double& screenPosY);
void processInput();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double mouseXPosition, double mouseYPosition);
bool portalIntersection(Camera& camera, Noise*& pActiveNoise, Noise*& pNextNoise, Terrain*& pActiveTerrain, Terrain*& pNextTerrain, Portal*& pActivePortal, Portal*& pInactivePortal, Grass& grass, const GLint& terrainShaderProgram, const GLint& defaultShaderProgram);

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

    glClearStencil(0x00);
    glEnable(GL_STENCIL_TEST);

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

void processInput()
{

    double mouseXPosition, mouseYPosition;
    glfwGetCursorPos(window, &mouseXPosition, &mouseYPosition);
    static bool rayButtonPressed = false;

    if(gEditMode < 2)
    {
        glm::vec3 rayDirection = getWorldRayFromCursor(mouseXPosition, mouseYPosition);
        pActiveTerrain->highlightRay(rayDirection, gEditMode);
        if(!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && rayButtonPressed)
        {
            rayButtonPressed = false;
        }
        else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && !rayButtonPressed)
        {
            rayButtonPressed = true;
            pActiveTerrain->modifyHeight();
        }
    }

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        camera.processMouse(float( lastMouseXPosition - mouseXPosition ), float( lastMouseYPosition - mouseYPosition ));
        glfwSetCursorPos(window, lastMouseXPosition, lastMouseYPosition);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        lastMouseXPosition = mouseXPosition;
        lastMouseYPosition = mouseYPosition;
    }

    if(glfwGetKey(window, GLFW_KEY_W )  == GLFW_PRESS) camera.moveForward( float(frameTime) );
    if(glfwGetKey(window, GLFW_KEY_S )  == GLFW_PRESS) camera.moveBack( float(frameTime) );
    if(glfwGetKey(window, GLFW_KEY_A )  == GLFW_PRESS) camera.moveLeft( float(frameTime) );
    if(glfwGetKey(window, GLFW_KEY_D )  == GLFW_PRESS) camera.moveRight( float(frameTime) );

    if(gEditMode == 3) camera.setHeight( pActiveTerrain->getHeightOfPosition(camera.getPosition()) + 0.5f );

}

glm::vec3 getWorldRayFromCursor(const double& screenPosX, const double& screenPosY)
{
    float x = (2.0f * screenPosX) / wWidth - 1.0f;
    float y = 1.0f - (2.0f * screenPosY) / wHeight;
    float z = -1.0f;

    glm::vec3 norm3DCoordsRay(x, y, z);
    glm::vec4 homogenCoordsRay(norm3DCoordsRay, 1.0);

    glm::vec4 camCoordsRay = glm::inverse(camera.getProjectionMatrix()) * homogenCoordsRay;

    camCoordsRay = glm::vec4(camCoordsRay.x, camCoordsRay.y, -1.0, 0.0);

    glm::vec3 worldCoordsRay = glm::vec3(glm::normalize(glm::inverse(camera.getViewMatrix()) * camCoordsRay));

    return worldCoordsRay;

    /* camera.update(); */
    /* float depth; */
    /* glReadPixels(screenPosX, screenPosY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth); */
    /* glm::vec3 windowCoords(static_cast<float>(screenPosX), static_cast<float>(wHeight) - static_cast<float>(screenPosY), depth); */
    /* glm::vec4 viewport(0.0f, 0.0f, wWidth, wHeight); */

    /* return glm::unProject(windowCoords, camera.getViewMatrix(), camera.getProjectionMatrix(), viewport); */
    /* return glm::unProject(windowCoords, glm::mat4(1.0f), camera.getProjectionMatrix(), viewport); */

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
    /* glfwSetCursorPosCallback(window, mouse_callback); */
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeCallback( window, resize_callback );
    /* glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1); */
    /* glfwSetInputMode(window, GLFW_STICKY_KEYS, 1); */

    /* glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); */
    glfwSetCursorPos(window, wWidth / 2, wHeight / 2);

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
     * Terrain Pointer: pActiveTerrain, pNextTerrain
     * Portal Pointer: pActivePortal, pInactivePortal
     *
     */
    Terrain *pNextTerrain;
    Portal *pActivePortal, *pInactivePortal;
    Noise *pActiveNoise, *pNextNoise;

    /*
     * Setup initial terrains and noise
     * First terrain uses perlin noise
     * Second uses worley noise
     * Use common parameters at current state of development
     *
     */

    // Common parameters
    int noiseDimX = 256, noiseDimY = 256, noiseDimZ = 1;
    int seed = 42, octaves = 16;
    double frequency = 8.0, amplitude = 4.0;

    // Setup perlin noise
    pActiveNoise = new PerlinNoise();
    pActiveNoise->setParams(noiseDimX, noiseDimY, seed);
    pActiveNoise->setOctavesFreqAmp(octaves, frequency, amplitude);

    Noise *pActiveNoise2 = new WorleyNoise();
    pActiveNoise2->setParams(noiseDimX, noiseDimY, seed);
    pActiveNoise2->setOctavesFreqAmp(octaves, frequency, amplitude);
    pActiveNoise2->generateNoiseImage();
    pActiveNoise2->saveToFile("WorleyNoise.tga");


    // Setup initial terrain
    pActiveTerrain = new Terrain(terrainShaderProgram, noiseDimX, noiseDimY, &camera, pActiveNoise);
    /* pActiveTerrain->enableNormals(); */
    pActiveTerrain->enableNormalMap();
    pActiveTerrain->computeTerrain();
    /* pActiveTerrain->addNoise(pActiveNoise2->getDataVector()); */
    pActiveTerrain->build();
    pActiveTerrain->genHeightMapTexture();
    pActiveTerrain->saveNoiseToFile("Terrain1.tga");
    pActiveTerrain->linkHeightMapTexture(defaultShaderProgram);
    /* pActiveTerrain->debug(); */

    // Setup worled (cell) noise for second terrain
    pNextNoise = new PerlinNoise();
    pNextNoise->setParams(noiseDimX, noiseDimY, seed);
    pNextNoise->setOctavesFreqAmp(octaves, frequency, amplitude);

    // Setup initial inactive terrain (in portal)
    pNextTerrain = new Terrain(terrainShaderProgram, noiseDimX, noiseDimY, &camera, pNextNoise);
    pNextTerrain->enableNormals();
    pNextTerrain->computeTerrain();
    pNextTerrain->build();
    pNextTerrain->genHeightMapTexture();
    pNextTerrain->saveNoiseToFile("Terrain2.tga");

    /*
     * Setup Portals
     * Each terrain will have one inactive portal (the portal where user came from)
     * and an active portal to teleport to the next terrain
     *
     */

    // Portal
    pActivePortal = new Portal(defaultShaderProgram);
    pActivePortal->init( &camera, pActiveTerrain, pNextTerrain );

    pInactivePortal = new Portal(defaultShaderProgram);
    pInactivePortal->init( &camera, pActiveTerrain, pNextTerrain );
    pInactivePortal->deaktivate();

    /*
     * Setup further terrain properties like skydome, grass, ...
     *
     */

    // Skydome noise parameters

    int noiseSkyDimX = 256, noiseSkyDimY = 256, noiseSkyDimZ = 32;
    seed = 245, octaves = 4, frequency = 4.0, amplitude = 1;

    // Setup noise for clouds
    SimplexNoise pNoise3D(noiseSkyDimX, noiseSkyDimY, noiseSkyDimZ, seed);
    pNoise3D.setOctavesFreqAmp(octaves, frequency, amplitude);
    pNoise3D.setScale(true);
    pNoise3D.generateTileableNoiseImage(1);

    // Skydome initialization
    Skydome skydome(skydomeShaderProgram, &camera);
    skydome.generateGeometry(noiseDimX / 3, 32, 32);
    skydome.loadTexture(pNoise3D.getTextureData(), noiseSkyDimX, noiseSkyDimY, noiseSkyDimZ);
    skydome.setBuffers();

    // Skydome 2 initialization
	Skydome skydome2(skydomeShaderProgram, &camera);
	skydome2.generateGeometry(noiseDimX / 3 - 5, 32, 32);
	skydome2.loadTexture(pNoise3D.getTextureData(), noiseSkyDimX, noiseSkyDimY, noiseSkyDimZ);
	skydome2.setBuffers();

	// Skydome 3 initialization
	Skydome skydome3(skydomeShaderProgram, &camera);
	skydome3.generateGeometry(noiseDimX / 3 - 10, 32, 32);
	skydome3.loadTexture(pNoise3D.getTextureData(), noiseSkyDimX, noiseSkyDimY, noiseSkyDimZ);
	skydome3.setBuffers();

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
    vec3 sphereTranslation = vec3(0.0, 10.0, 0.0);
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
            //here update game
            glfwPollEvents();

            processInput();

            newTime += frameTime;

            loops++;

            /*
             *
             * Check for portal intersection
             *
             */
            portalIntersection(camera, pActiveNoise, pNextNoise, pActiveTerrain, pNextTerrain, pActivePortal, pInactivePortal, grass, terrainShaderProgram, defaultShaderProgram);
        }


        //update Frame

        // Clear buffers
        glStencilMask(0xFF);
        glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);

        // Recalculate camera matrices
        camera.update();

        /***************************************************
         * Start rendering
         ***************************************************/

        pActivePortal->enableStencil();

        /***************************************************
         * Draw main scene
         ***************************************************/
        pActivePortal->renderOutside();

        // Portal
        pActivePortal->drawPortal();
        pInactivePortal->drawPortal();

        // Sphere
        model.setProjection(camera.getProjectionMatrix());
        model.setView(camera.getViewMatrix());
        model.draw();

        // Active terrain
        pActiveTerrain->setGrid(gDrawGrid);
        pActiveTerrain->draw();

        // Grass
        grass.setViewAndProjectionMatrix(camera.getViewMatrix(), camera.getProjectionMatrix());
        grass.draw();

        // Skydome
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        skydome.draw();
        skydome2.draw();
        skydome3.draw();
        glDisable(GL_BLEND);


        // Draw in stencil pattern (in portal)


        /***************************************************
         * Draw inside portal.
         ***************************************************/
        pActivePortal->renderInside();

        // Portal
        pActivePortal->drawPortal();
        pInactivePortal->drawPortal();

        // Sphere
        model.setProjection(camera.getProjectionMatrix());
        model.setView(camera.getViewMatrix());
        model.draw();

        // Inactive or next terrain
        pNextTerrain->setGrid(gDrawGrid);
        pNextTerrain->draw();

        // Grass
        grass.setViewAndProjectionMatrix(camera.getViewMatrix(), camera.getProjectionMatrix());
        grass.draw();

        // Skydome
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		skydome.draw();
		skydome2.draw();
		skydome3.draw();
		glDisable(GL_BLEND);


        // Disable stencil test


        /***************************************************
         * Disable stencil test and reset camera
         ***************************************************/
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
    delete pNextTerrain;
    delete pActivePortal;
    delete pInactivePortal;
    delete pActiveNoise;
    delete pNextNoise;

    // Programs
    glDeleteProgram(defaultShaderProgram);
    glDeleteProgram(grassShaderProgram);
    glDeleteProgram(terrainShaderProgram);
    glDeleteProgram(skydomeShaderProgram);
    glDeleteProgram(cloudsShaderProgram);

}

bool portalIntersection(Camera& camera, Noise*& pActiveNoise, Noise*& pNextNoise, Terrain*& pActiveTerrain, Terrain*& pNextTerrain, Portal*& pActivePortal, Portal*& pInactivePortal, Grass& grass, const GLint& terrainShaderProgram, const GLint& defaultShaderProgram)
{

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
        int octaves = seed < 0 ? static_cast<int>(-seed) % 17 : static_cast<int>(seed) % 17;
        std::cout << "Octaves: "<< octaves << std::endl;
        double frequency = mod(seed, 16.0);
        double amplitude = mod(seed, 8.0);
        Noise* pNewNoise = new PerlinNoise();
        pNewNoise->setParams(noiseDimX, noiseDimY, seed);
        pNewNoise->setOctavesFreqAmp(octaves, frequency, amplitude);

        delete pActiveNoise;
        pActiveNoise    = pNextNoise;
        pNextNoise      = pNewNoise;

        std::cout << "Generate new terrain" << std::endl;
        Terrain* pNewTerrain = new Terrain(terrainShaderProgram, noiseDimX, noiseDimY, &camera, pNextNoise);
        pNewTerrain->enableNormals();
        pNewTerrain->computeTerrain();
        pNewTerrain->build();
        pNewTerrain->genHeightMapTexture();
        pNewTerrain->saveNoiseToFile("newTerrain_noise.tga");
        pNewTerrain->linkHeightMapTexture(defaultShaderProgram);
        pNewTerrain->draw();

        // swap terrains
        delete pActiveTerrain;
        pActiveTerrain  = pNextTerrain;
        pNextTerrain    = pNewTerrain;

        grass.setTerrainVao(pActiveTerrain->getVAO(), pActiveTerrain->getTotalIndices());
        std::cout << "Generate new portal" << std::endl;
        // Setup new portal
        Portal* pNewPortal = new Portal(defaultShaderProgram);
        pNewPortal->init( &camera, pActiveTerrain, pNextTerrain );

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
        case GLFW_KEY_E :
            if(action == GLFW_PRESS)
            {
                gEditMode = (gEditMode + 1) % 4;
                pActiveTerrain->unHighlightRay();
                if(gEditMode == 3 ) camera.setCamSpeed( 0.1 );
                else if(gEditMode == 0) camera.setCamSpeed( 10.1 );
            }
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

    }
        //mRight * (float)mDeltaTime * mCamSpeed
}

/* void mouse_callback(GLFWwindow* window, double mouseXPosition, double mouseYPosition, const Camera &camera) */
void mouse_callback(GLFWwindow* window, double mouseXPosition, double mouseYPosition)
{

}

/* void scroll_callback(GLFWwindow* window, double xoffset, double yoffset, const Camera &camera) */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    pActiveTerrain->changeModifyRadius(yoffset);
}
