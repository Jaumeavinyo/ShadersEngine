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

#include <GLFW/glfw3.h>

glm::mat4 transformPositionScale(const vec3& pos, const vec3& scaleFactors) {
    glm::mat4 transform = glm::translate(pos);
    transform = scale(transform, scaleFactors);
    return transform;
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

void cameraSetUp(App* app) {
    app->camera.cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);//zpositive = backwards
    app->camera.cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    app->camera.cameraDirection = glm::normalize(app->camera.cameraPos - app->camera.cameraTarget);//The name direction vector is not the best chosen name, since it is actually pointing in the reverse direction of what it is targeting.
    app->camera.cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), app->camera.cameraDirection));
    app->camera.cameraUp = glm::cross(app->camera.cameraDirection, app->camera.cameraRight);

    

    app->camera.viewTransform = glm::lookAt(app->camera.cameraPos, app->camera.cameraPos + (-app->camera.cameraDirection), app->camera.cameraUp);
    app->camera.projectionTransform = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

}

void modelTransform(App* app) {
    app->camera.modelTransform = glm::mat4(1.0f);
    app->camera.modelTransform = glm::scale(app->camera.modelTransform, glm::vec3(0.2, 0.2, 0.2));
}

void createVSLayout(Program& program) {
    GLint attrCount = 0;
    glUseProgram(program.handle);
    glGetProgramiv(program.handle, GL_ACTIVE_ATTRIBUTES, &attrCount);
  
    glCheckError();


    for (int i = 0; i < attrCount; i++) {
        const int bufferSize = 256; // adjust buffer size as needed
        GLsizei length;
        GLint size;
        GLenum type;
        GLchar name[bufferSize];
        glGetActiveAttrib(program.handle, i, bufferSize, &length, &size, &type, name);
        glCheckError();
        unsigned int attribLocation = glGetAttribLocation(program.handle, name);
        program.VSLayout.Push(length, size, type, *name, attribLocation);
    }
    glUseProgram(0);
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

void setUniformBuffer(App* app,std::string name) {
   
    app->LocalParams.name = name;
    
    GLint maxUniformBufferSize;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBufferSize);
    GLint uniformBlockAligment;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniformBlockAligment);

    glGenBuffers(1, &app->LocalParams.BufferHandle);
    glBindBuffer(GL_UNIFORM_BUFFER, app->LocalParams.BufferHandle);
    glBufferData(GL_UNIFORM_BUFFER, maxUniformBufferSize, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    unsigned int blockOffset = 0;
    unsigned int blockSize = sizeof(glm::mat4) * 2;
    glBindBufferRange(GL_UNIFORM_BUFFER, BINDING(1), app->LocalParams.BufferHandle, blockOffset, blockSize);
   
}

void updateUniformBuffers(App* app,unsigned int entityIDx,glm::mat4 worldMat,glm::mat4 worldViewProjection) {
    
    
    glBindBuffer(GL_UNIFORM_BUFFER, app->LocalParams.BufferHandle);
    unsigned char* bufferData = (unsigned char*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    unsigned int bufferHead = 0;

    memcpy(bufferData + bufferHead, glm::value_ptr(worldMat), sizeof(glm::mat4));
    bufferHead += sizeof(glm::mat4);

    memcpy(bufferData + bufferHead, glm::value_ptr(worldViewProjection), sizeof(glm::mat4));
    bufferHead += sizeof(glm::mat4);

    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


           
        
    
}

void sendUniforms(App* app,Program& program, unsigned int entityIDx) {

    //Model model = app->models[modelIDx];
    Entity entity = app->entities[entityIDx];
    unsigned int textureLocation = glGetUniformLocation(program.handle, "uTexture");
    glUniform1i(textureLocation, 0);
    
    // MATRIX UNIFORMS
    app->camera.viewTransform = glm::lookAt(app->camera.cameraPos, app->camera.cameraPos + (-app->camera.cameraDirection), app->camera.cameraUp);
    entity.pos.x += glm::sin((float)glfwGetTime());
    entity.worldMat = transformPositionScale(entity.pos, vec3(0.5));
    entity.worldMat = glm::rotate(entity.worldMat, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 worldViewProjection = app->camera.projectionTransform * app->camera.viewTransform * entity.worldMat;

    updateUniformBuffers(app,entityIDx, entity.worldMat, worldViewProjection);



    //unsigned int worldMatLocation = glGetUniformLocation(program.handle, "worldMat");
    //glUniformMatrix4fv(worldMatLocation, 1, GL_FALSE, glm::value_ptr(worldMat));
    //glCheckError();

    ////PROJECTION TRANSFORM
    //unsigned int worldViewProjectionLocation = glGetUniformLocation(program.handle, "worldViewProjection");
    //glUniformMatrix4fv(worldViewProjectionLocation, 1, GL_FALSE, glm::value_ptr(worldViewProjection));
    //glCheckError();
}

void Init(App* app)
{
    cameraSetUp(app);
    modelTransform(app);

    //const char* name = "cube/Crate1.obj";
    const char* name = "Patrick/Patrick.obj";
    app->modelIDx = LoadModel(app, name);
    /*app->models[app->modelIDx].pos = glm::vec3(0.0, 0.0, -10.0);*/

    Entity entity0 = Entity{glm::mat4(),glm::vec3(6.0,0.0,-15.0),app->modelIDx};
    Entity entity1 = Entity{ glm::mat4(),glm::vec3(1.0,0.0,-10.0),app->modelIDx };
    Entity entity2 = Entity{ glm::mat4(),glm::vec3(-2.0,0.0,-5.0),app->modelIDx };
    app->entities.push_back(entity0);
    app->entities.push_back(entity1);
    app->entities.push_back(entity2);


    //localParams uniform block
    std::string uniformName = "LocalParams";
    setUniformBuffer(app,uniformName);

    app->texturedMeshProgramIDx = LoadAndCreateProgram(app, "../Basic2.shader", app->shaderProgramsSrc);
    Program& TexturedMeshProgram = app->programs[app->texturedMeshProgramIDx];
    createVSLayout(TexturedMeshProgram);
    
    
    glEnable(GL_DEPTH_TEST);

    app->mode = Mode::Mode_TexturedQuad;
}



void Update(App* app, GLFWwindow* window)
{
    // You can handle app->input keyboard/mouse here
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        app->camera.cameraPos += cameraSpeed * -app->camera.cameraDirection;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        app->camera.cameraPos -= cameraSpeed * -app->camera.cameraDirection;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        app->camera.cameraPos -= glm::normalize(glm::cross(-app->camera.cameraDirection, app->camera.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        app->camera.cameraPos += glm::normalize(glm::cross(-app->camera.cameraDirection, app->camera.cameraUp)) * cameraSpeed;
    
}

void renderEntities(App*app, unsigned int entityIDx) {
    Program texturedMeshProgram = app->programs[app->texturedMeshProgramIDx];
    glUseProgram(texturedMeshProgram.handle);
    glCheckError();
    Model& model = app->models[app->entities[entityIDx].modelIDx];
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

        sendUniforms(app, texturedMeshProgram, entityIDx);

        SubMesh& submesh = mesh.submeshes[i];
        glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, (void*)(unsigned long long int)submesh.indexOffset);
        glCheckError();

    }
}

void Render(App* app)
{
    switch (app->mode)
    {
        case Mode_TexturedQuad:
        {
            glClearColor(0.2, 0.2, 0.2, 1.);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            for (unsigned int i = 0; i < app->entities.size(); i++) {
                renderEntities(app, i);
            }
            

        }
            break;

        
    }
}

