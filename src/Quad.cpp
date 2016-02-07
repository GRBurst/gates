#include "Quad.h"

Quad::Quad()
    : quadVAO(0)
    , quadVBO(0)
    , mQuadProgram()
    , mFirstCall(1)
    , mDim(3)
{
    Shader mQuadShader;
    mQuadShader.loadShader("../src/shader/quad.vs", Shader::VERTEX);
    mQuadShader.loadShader("../src/shader/quad.fs", Shader::FRAGMENT);
    this->mQuadProgram = mQuadShader.linkShaders();
}

Quad::~Quad()
{}

void Quad::init()
{

    GLfloat quadVertices[] = {
        // Positions        // Texture Coords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
}

void Quad::render()
{
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Quad::render(Texture& texture)
{
    render(mQuadProgram, texture);
}

void Quad::render(const GLint& shader, Texture& texture)
{
    glUseProgram(shader);
    if(mFirstCall)
    {
        texture.linkTexture(shader, "sTexture");
        mFirstCall = 0;
    }
    texture.bind();
    glUniform1i(glGetUniformLocation(shader, "uDim"), mDim);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
