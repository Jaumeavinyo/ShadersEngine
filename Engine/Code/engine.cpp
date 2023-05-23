//
// engine.cpp : Put all your graphics stuff in this file. This is kind of the graphics module.
// In here, you should type all your OpenGL commands, and you can also type code to handle
// input platform events (e.g to move the camera or react to certain shortcuts), writing some
// graphics related GUI options, and so on.
//

#include "engine.h"
#include <imgui.h>


#include <iostream>
#include <fstream>
#include <sstream>


#include "..\errorHandler.h"
#include "../GeometryLoader.h"


GLuint CreateProgramFromSource(String programSource, const char* shaderName)
{
    GLchar  infoLogBuffer[1024] = {};
    GLsizei infoLogBufferSize = sizeof(infoLogBuffer);
    GLsizei infoLogSize;
    GLint   success;

    char versionString[] = "#version 330\n";//PREV 430
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
    glCheckError();
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCheckError();
    glCompileShader(id);
    glCheckError();

    //ERROR HANDLING HERE
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    glCheckError();
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length*sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        ELOG("Failed to compile %i shader! problem: %s", type, message);
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
    glCheckError();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glCheckError();
    glAttachShader(program, fs);
    glCheckError();
    glLinkProgram(program);
    glCheckError();
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
    glCheckError();
    
   

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

//returns the program position in programs array
unsigned int LoadAndCreateProgram(App*app,std::string filePath, ShaderProgramSource shaderProgramsSrc) {
    
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (glslVersion != nullptr) {
        // Print or store the GLSL version
        printf("GLSL version: %s\n", glslVersion);
    }
    else {
        // Failed to retrieve GLSL version
        printf("Failed to retrieve GLSL version\n");
    }
    
    app->shaderProgramsSrc = parseShader(filePath);
    Program program = {};
    program.handle = createShader(app->shaderProgramsSrc.vertexSrc, app->shaderProgramsSrc.fragmentSrc);
    program.filepath = filePath;
    program.programName = filePath;
    program.lastWriteTimestamp = GetFileLastWriteTimestamp(filePath.c_str());
    app->programs.push_back(program);

    return app->programs.size() - 1;
}



GLuint FindVAO(Mesh& mesh, unsigned int submeshIndex, const Program& program) {
    SubMesh& submesh = mesh.submeshes[submeshIndex];

    //try find vao for the submesh/program

    for (unsigned int i = 0; i < submesh.vaos.size(); i++) {
        if (submesh.vaos[i].programHandle == program.handle) {
            return submesh.vaos[i].handle;
        }
    }

    //create vao for this submesh program
    GLuint vaoHandle = 0;
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glBindBuffer(GL_ARRAY_BUFFER,mesh.VBO_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO_handle);

    

    for (unsigned int i = 0; i < program.VSLayout.getElements().size(); ++i) {
        bool attribLinked = false;
        for (unsigned int j = 0; j < submesh.VBLayout.getElements().size(); ++j) {
            if (program.VSLayout.getElements()[i].location == submesh.VBLayout.getElements()[j].location) {

                unsigned int index = submesh.VBLayout.getElements()[j].location;
                unsigned int count = submesh.VBLayout.getElements()[j].count;
                unsigned int offset = submesh.VBLayout.getElements()[j].elementOffset + submesh.vertexOffset;
                unsigned int stride = submesh.VBLayout.getStride();

                auto element = submesh.VBLayout.getElements()[j];
                              
                glVertexAttribPointer(index, count, element.type, GL_FALSE/*element.normalized*/, stride, (const void*)offset);
                glCheckError();
                glEnableVertexAttribArray(index);
                glCheckError();

                
                attribLinked = true;
                break;
                
            }
        }
        assert(attribLinked);//submesh VBLayout should have attribute for each VSLayout attribute
    }

    glBindVertexArray(0);


    VAO vao = { vaoHandle,program.handle };
    submesh.vaos.push_back(vao);

    return vaoHandle;

}


void Init(App* app)
{

    // TRANSFORMS   TRANSFORMS  TRANSFORMS  TRANSFORMS

    //MODEL
    app->camera.modelTransform = glm::mat4(1.0f);
    app->camera.modelTransform = glm::scale(app->camera.modelTransform, glm::vec3(0.2, 0.2, 0.2));

    //VIEW
    app->camera.viewTransform = glm::mat4(1.0f);
    app->camera.viewTransform = glm::translate(app->camera.viewTransform, glm::vec3(0.0f, 0.0f, -30.0f));

    //PROJECTION
    app->camera.projectionTransform = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    //const char* name = "cube/Crate1.obj";
    const char* name = "Patrick/Patrick.obj";
    app->modelIDx = LoadModel(app, name);
        

    app->texturedMeshProgramIDx = LoadAndCreateProgram(app, "../Basic.shader", app->shaderProgramsSrc);
    Program& TexturedMeshProgram = app->programs[app->texturedMeshProgramIDx];
   
    
    
    GLint attrCount = 0;
    glGetProgramiv(TexturedMeshProgram.handle, GL_ACTIVE_ATTRIBUTES, &attrCount);
    glCheckError();
    
    
    for (int i = 0; i < attrCount; i++) {
        const int bufferSize = 256; // adjust buffer size as needed
        GLsizei length;
        GLint size;
        GLenum type;
        GLchar name[bufferSize];
        glGetActiveAttrib(TexturedMeshProgram.handle, i, bufferSize, &length, &size, &type, name);
        glCheckError();
        unsigned int attribLocation = glGetAttribLocation(TexturedMeshProgram.handle, name);
        TexturedMeshProgram.VSLayout.Push(length, size, type, *name,attribLocation);
    }

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
            glClearColor(0.2, 0.2, 0.2, 1.);
            Program texturedMeshProgram = app->programs[app->texturedMeshProgramIDx];
            glUseProgram(texturedMeshProgram.handle);
            glCheckError();
            Model& model = app->models[app->modelIDx];
            Mesh& mesh = app->meshes[model.meshIDx];

            for (unsigned int i = 0; i < mesh.submeshes.size(); ++i) {
                GLuint vao = FindVAO(mesh, i, texturedMeshProgram);
                glBindVertexArray(vao);
                glCheckError();
                unsigned int submeshMatIDx = model.materialIDx[i];
                Material& submeshMaterial = app->materials[submeshMatIDx];
                
                glActiveTexture(GL_TEXTURE0);
                glCheckError();
                glBindTexture(GL_TEXTURE_2D, app->textures[submeshMaterial.albedoTextureIDx].handle);
                glCheckError();
                unsigned int textureLocation = glGetUniformLocation(texturedMeshProgram.handle, "uTexture");
                glUniform1i(textureLocation, 0);
                
                //MODEL TRANSFORM
                app->camera.modelTransform = glm::rotate(app->camera.modelTransform, glm::radians(0.2f), glm::vec3(0.0, 1.0, 0.0));
                unsigned int transformLocation = glGetUniformLocation(texturedMeshProgram.handle, "modelTransform");
                glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(app->camera.modelTransform));
                glCheckError();
                //VIEW TRANSFORM
                unsigned int transformView = glGetUniformLocation(texturedMeshProgram.handle, "viewTransform");
                glUniformMatrix4fv(transformView, 1, GL_FALSE, glm::value_ptr(app->camera.viewTransform));
                glCheckError();
                //PROJECTION TRANSFORM
                unsigned int transformProjection= glGetUniformLocation(texturedMeshProgram.handle, "projectionTransform");
                glUniformMatrix4fv(transformProjection, 1, GL_FALSE, glm::value_ptr(app->camera.projectionTransform));
                glCheckError();


                SubMesh& submesh = mesh.submeshes[i];
                glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, (void*)(unsigned long long int)submesh.indexOffset);
                glCheckError();
            }

        }
            break;

        
    }
}

