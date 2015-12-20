# G.A.T.E.S.

## Dependencies:
- glew
- glfw3
- glm
- OpenGL 4.0
- cmake 2.8

## Build G.A.T.E.S. using cmake
1. Create a build folder in project folder

        mkdir build-x86_64

2. Navigate in the created folder

        cd build-x86_64

3. Run ccmake and choose appropriate option. Default compiler is g++, you can choose clang++ within ccmake.

        ccmake ..

    - After choosing compiler and build type, run configure by typing "c"
    - Generate makefiles by typing "g" for generation

## Compile G.A.T.E.S.
Compile by simply running make in the build folder

    make

## Run G.A.T.E.S.
Make created an executeable called "gates". You can run gates from terminal in the build folder by running it with

    ./gates



