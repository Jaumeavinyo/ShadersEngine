//
// engine.h: This file contains the types and functions relative to the engine.
//

#pragma once



#include "platform.h"
#include <glad/glad.h>
#include "..\GameObject.h"




typedef glm::vec2  vec2;
typedef glm::vec3  vec3;
typedef glm::vec4  vec4;
typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;



struct ShaderProgramSource {
    std::string vertexSrc;
    std::string fragmentSrc;
};

struct Image
{
    void* pixels;
    ivec2 size;
    i32   nchannels;
    i32   stride;
};

struct Texture
{
    GLuint      handle;
    std::string filepath;
};

struct Program
{
    GLuint             handle;
    std::string        filepath;
    std::string        programName;
    u64                lastWriteTimestamp; // What is this for?
};

struct Vertex {
    glm::vec3 pos;
    //glm::vec2 uv;
};


enum Mode
{
    Mode_TexturedQuad,
    Mode_Count
};

unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
unsigned int compileShader(unsigned int type, const std::string& source);
ShaderProgramSource parseShader(std::string filePath);

struct App
{
    // Loop
    f32  deltaTime;
    bool isRunning;

    // Input
    Input input;

    // Graphics
    char gpuName[64];
    char openGlVersion[64];

    ivec2 displaySize;

    std::vector<Texture>  textures;
    std::vector<Program>  programs;

    // program indices
    u32 texturedGeometryProgramIdx;
    
    // texture indices
    u32 diceTexIdx;
    u32 whiteTexIdx;
    u32 blackTexIdx;
    u32 normalTexIdx;
    u32 magentaTexIdx;

    // Mode
    Mode mode;

    // Embedded geometry (in-editor simple meshes such as
    // a screen filling quad, a cube, a sphere...)
    GLuint embeddedVertices;
    GLuint embeddedElements;

    // Location of the texture uniform in the textured quad shader
    GLuint programUniformTexture;
   
    std::vector<Vertex> vertices = {
       {glm::vec3(-0.5,-0.5,0.0)/*,glm::vec2(0.0,0.0)*/},
       {glm::vec3(0.5,-0.5,0.0)/*,glm::vec2(1.0,0.0)*/},
       {glm::vec3(0.5,0.5,0.0)/*,glm::vec2(1.0,1.0)*/},
       {glm::vec3(-0.5,0.5,0.0)/*,glm::vec2(0.0,1.0)*/}

    };

    
    //MY OWN DATA
    float vertex[8] = {
       -1.0f,-1.0f,
        1.0f,-1.0f,
        0.9f, 0.9f,
       -0.9f, 0.9f 
    };

    unsigned int indices[6] = {
        0,1,2,
        2,3,0
    };

    ShaderProgramSource shaderProgramsSrc;
    unsigned int shader;



    std::vector<GameObject*> gameObjects;

};

void Init(App* app);

void Gui(App* app);

void Update(App* app);

void Render(App* app);




