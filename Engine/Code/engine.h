//
// engine.h: This file contains the types and functions relative to the engine.
//

#pragma once



#include "platform.h"
#include <glad/glad.h>
#include "..\GameObject.h"
#include "../GLclassesAndStructs.h"


unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
unsigned int compileShader(unsigned int type, const std::string& source);
ShaderProgramSource parseShader(std::string filePath);


void LoadAndCreateProgram(std::string filePath, ShaderProgramSource shaderProgramsSrc);

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
   
    unsigned int shader;

    //###########################################

    ShaderProgramSource shaderProgramsSrc;

    unsigned int texturedMeshProgramIDx; //the id (position) of the most recently created shader in programs vector
    std::vector<Program>  programs;
    
    
    std::vector<GameObject*> gameObjects;
    std::vector<Mesh> meshes;
    std::vector<Model> models;
    std::vector<Material> materials;
    std::vector<Texture>  textures;
   


   

};

void Init(App* app);

void Gui(App* app);

void Update(App* app);

void Render(App* app);




