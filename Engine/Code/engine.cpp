//
// engine.cpp : Put all your graphics stuff in this file. This is kind of the graphics module.
// In here, you should type all your OpenGL commands, and you can also type code to handle
// input platform events (e.g to move the camera or react to certain shortcuts), writing some
// graphics related GUI options, and so on.
//

#include "engine.h"
#include <imgui.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include "../Texture.h"

#include <iostream>
#include <fstream>
#include <sstream>


#include "..\errorHandler.h"
#include "..\MeshComponent.h"


GLuint CreateProgramFromSource(String programSource, const char* shaderName)
{
    GLchar  infoLogBuffer[1024] = {};
    GLsizei infoLogBufferSize = sizeof(infoLogBuffer);
    GLsizei infoLogSize;
    GLint   success;

    char versionString[] = "#version 430\n";
    char shaderNameDefine[128];
    sprintf(shaderNameDefine, "#define %s\n", shaderName);
    char vertexShaderDefine[] = "#define VERTEX\n";
    char fragmentShaderDefine[] = "#define FRAGMENT\n";

    const GLchar* vertexShaderSource[] = {
        versionString,
        shaderNameDefine,
        vertexShaderDefine,
        programSource.str
    };
    const GLint vertexShaderLengths[] = {
        (GLint) strlen(versionString),
        (GLint) strlen(shaderNameDefine),
        (GLint) strlen(vertexShaderDefine),
        (GLint) programSource.len
    };
    const GLchar* fragmentShaderSource[] = {
        versionString,
        shaderNameDefine,
        fragmentShaderDefine,
        programSource.str
    };
    const GLint fragmentShaderLengths[] = {
        (GLint) strlen(versionString),
        (GLint) strlen(shaderNameDefine),
        (GLint) strlen(fragmentShaderDefine),
        (GLint) programSource.len
    };

    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, ARRAY_COUNT(vertexShaderSource), vertexShaderSource, vertexShaderLengths);
    glCompileShader(vshader);
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vshader, infoLogBufferSize, &infoLogSize, infoLogBuffer);
        ELOG("glCompileShader() failed with vertex shader %s\nReported message:\n%s\n", shaderName, infoLogBuffer);
    }

    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, ARRAY_COUNT(fragmentShaderSource), fragmentShaderSource, fragmentShaderLengths);
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fshader, infoLogBufferSize, &infoLogSize, infoLogBuffer);
        ELOG("glCompileShader() failed with fragment shader %s\nReported message:\n%s\n", shaderName, infoLogBuffer);
    }

    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vshader);
    glAttachShader(programHandle, fshader);
    glLinkProgram(programHandle);
    glGetProgramiv(programHandle, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programHandle, infoLogBufferSize, &infoLogSize, infoLogBuffer);
        ELOG("glLinkProgram() failed with program %s\nReported message:\n%s\n", shaderName, infoLogBuffer);
    }

    glUseProgram(0);

    glDetachShader(programHandle, vshader);
    glDetachShader(programHandle, fshader);
    glDeleteShader(vshader);
    glDeleteShader(fshader);

    return programHandle;
}

u32 LoadProgram(App* app, const char* filepath, const char* programName)
{
    String programSource = ReadTextFile(filepath); //returns a string with the shader

    Program program = {};
    program.handle = CreateProgramFromSource(programSource, programName);//returns uint for shader program location
    program.filepath = filepath;
    program.programName = programName;
    program.lastWriteTimestamp = GetFileLastWriteTimestamp(filepath);
    app->programs.push_back(program);

    return app->programs.size() - 1;
}



void Gui(App* app)
{
    ImGui::Begin("Info");
    ImGui::BeginChild("program data");
    ImGui::Text("FPS: %f", 1.0f / app->deltaTime);
    ImGui::Text("OpenGlVersion: %s", glGetString(GL_VERSION));
    ImGui::Text("OpenGlRenderer: %s", glGetString(GL_RENDERER));

    int num_extensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
    for (int i = 0; i < num_extensions; i++) {
        ImGui::Text("GL_EXTENSION_%i : %s", i, glGetStringi(GL_EXTENSIONS, GLuint(i)));
    }

    ImGui::EndChild();
    ImGui::End();
}

ShaderProgramSource parseShader(std::string filePath) {
    std::ifstream stream(filePath);//opens the file

    enum class ShaderType{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(),ss[1].str() };

}

unsigned int compileShader( unsigned int type, const std::string& source) {

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //ERROR HANDLING HERE
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length*sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        ELOG("Failed to compile %i shader!", type);
        glDeleteShader(id);
        return 0;
    }
    else if (result == GL_TRUE) {
        ELOG("Success to compile %i shader!", type);
    }

    return id;
}
unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {

    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    //HANDLE LINKING ERRORS
    int linkSuccess = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);

    if (linkSuccess == GL_FALSE) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        // Allocate memory for the log
        char* logMessage = new char[logLength];
        glGetProgramInfoLog(program, logLength, nullptr, logMessage);
        ELOG("program linking failed, error: %s", logMessage);
        delete[] logMessage;
    }

    glValidateProgram(program);
   
    
   

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Init(App* app)
{


    app->gameObjects.push_back(new GameObject());

    


    //VertexBufferLayout layout;
    //layout.Push<float>(3);//first element of the stride: 2 floats for position
    //layout.Push<float>(2);//second element 2 floats for uvg texcorrds
    //Mesh* mesh = new Mesh(app->vertices, layout, app->indices, 6);

    //SHADER
    //app->shaderProgramsSrc = parseShader("Basic.shader");
    //app->shader = createShader(app->shaderProgramsSrc.vertexSrc, app->shaderProgramsSrc.fragmentSrc);
    //glUseProgram(app->shader);
    //glCheckError();

    //int location1 = glGetUniformLocation(app->shader, "u_Texture");
    //assert(location1 != -1);
    //glUniform1i(location1, 0);
   
    ////!SHADER
    //

    //glUseProgram(0);
    //glCheckError();


    //Material* mat = new Material();

    ////TEXTURE LOADING
    //mat->textureID = LoadTexture2D(app, "WorkingDir/dice.png");
    //glActiveTexture(GL_TEXTURE0);
    //glCheckError();
    ////!TEXTURE LOADING

    //

    //std::string name = "MeshComponent";
    //const char* resourcePath = "patrick.obj";
    //MeshComponent* meshComp = new MeshComponent(app->gameObjects[0], name, mat, resourcePath);
    //

    //app->gameObjects[0]->addComponent(meshComp);
    //dynamic_cast<MeshComponent*>(app->gameObjects[0]->getComponent(0))->Init();

    app->mode = Mode::Mode_TexturedQuad;
}



void Update(App* app)
{
    // You can handle app->input keyboard/mouse here

    for (int i = 0; i < app->gameObjects.size(); i++) {
        app->gameObjects[i]->Update();
    }
}

void Render(App* app)
{
    switch (app->mode)
    {
        case Mode_TexturedQuad:
        {
            //glUseProgram(app->shader);
            //glCheckError();
            //glClearColor(0.2, 0.2, 0.2, 1.0);
            //glCheckError();
            //for (int i = 0; i < app->gameObjects.size(); i++) {
            //    if (app->gameObjects[i]->getComponent(i)->getName() == "MeshComponent") {
            //        MeshComponent* meshComp = dynamic_cast<MeshComponent*>(app->gameObjects[i]->getComponent(i));//get mesh from component list and bind va and ib
            //        
            //        
            //        glBindTexture(GL_TEXTURE_2D,app->textures[meshComp->getMaterial()->textureID].handle );
            //        glCheckError();
            //        glBindVertexArray(meshComp->getMesh()->getVAO());
            //        glCheckError();

            //        glDrawElements(GL_TRIANGLES, meshComp->getMesh()->indexCount, GL_UNSIGNED_INT, nullptr);//nullptr bc we already passed indices with the ibo glBufferData() func
            //        glCheckError();
            //    }
            //      
            //}
            

          
        }
            break;

        
    }
}

