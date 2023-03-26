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
#include <iostream>



using namespace std;
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

Image LoadImage(const char* filename)
{
    Image img = {};
    stbi_set_flip_vertically_on_load(true);
    img.pixels = stbi_load(filename, &img.size.x, &img.size.y, &img.nchannels, 0);
    if (img.pixels)
    {
        img.stride = img.size.x * img.nchannels;
    }
    else
    {
        ELOG("Could not open file %s", filename);
    }
    return img;
}

void FreeImage(Image image)
{
    stbi_image_free(image.pixels);
}

GLuint CreateTexture2DFromImage(Image image)
{
    GLenum internalFormat = GL_RGB8;
    GLenum dataFormat     = GL_RGB;
    GLenum dataType       = GL_UNSIGNED_BYTE;

    switch (image.nchannels)
    {
        case 3: dataFormat = GL_RGB; internalFormat = GL_RGB8; break;
        case 4: dataFormat = GL_RGBA; internalFormat = GL_RGBA8; break;
        default: ELOG("LoadTexture2D() - Unsupported number of channels");
    }

    GLuint texHandle;
    glGenTextures(1, &texHandle);
    glBindTexture(GL_TEXTURE_2D, texHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.size.x, image.size.y, 0, dataFormat, dataType, image.pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texHandle;
}

u32 LoadTexture2D(App* app, const char* filepath)
{
    for (u32 texIdx = 0; texIdx < app->textures.size(); ++texIdx)
        if (app->textures[texIdx].filepath == filepath)
            return texIdx;

    Image image = LoadImage(filepath);

    if (image.pixels)
    {
        Texture tex = {};
        tex.handle = CreateTexture2DFromImage(image);
        tex.filepath = filepath;

        u32 texIdx = app->textures.size();
        app->textures.push_back(tex);

        FreeImage(image);
        return texIdx;
    }
    else
    {
        return UINT32_MAX;
    }
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


static unsigned int compileShader( unsigned int type, const std::string& source) {

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
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {

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
  
   
   
    glGenBuffers(1, &app->vertexBufferObj);//create "1" buffer in "&buffer"
    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, app->vertexBufferObj);//select as an "GL_ARRAY_BUFFER" of info the "buffer" we are binding
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), app->positions, GL_STATIC_DRAW);//create a space of memory in the binded buffer as "GL_ARRAY_BUFFER" size of "6 floats", bytes in positions and type of draw
    glCheckError();
    //index for the attribute position (attribute 1 = pos(3float), attr 2 = uv(2float),attr 3 = normal(3float),size is the ammount of elements in the attribute, 3dpos will be 3, type of data "GL_FLOAT",
    // normalized means that if we pass an rgb with values 0-255 convert it to 0.to  1. stride is ammount of bytes between each vertex with all its attributes inside
    // the final pointer is the offset, the position of the attribute in every vertex
    //call for every attribute in bufferr
   
    glGenVertexArrays(1, &app->vertexArrayObj);
    glCheckError();
    glBindVertexArray(app->vertexArrayObj);
    glCheckError();
    glEnableVertexAttribArray(0);//enable attr 0, created avobe
    glCheckError();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);
    glCheckError();
    
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "gl_Position = position;\n"
        "}\n"          
        ;
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(1.0,0.5,0.0,1.0);\n"
        "}\n"
        ;
    
    unsigned int shader = createShader(vertexShader,fragmentShader);
    glUseProgram(shader);
    glCheckError();
   
    
    GLuint error;
    
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGl error: " << error << "-"<< std::endl;
    }

    app->mode = Mode::Mode_TexturedQuad;
}



void Update(App* app)
{
    // You can handle app->input keyboard/mouse here
}

void Render(App* app)
{
    switch (app->mode)
    {
        case Mode_TexturedQuad:
        {

            // TODO: Draw your textured quad here!
            // - clear the framebuffer
            // - set the viewport
            // - set the blending state
            // - bind the texture into unit 0
            // - bind the program 
            //   (...and make its texture sample from unit 0)
            // - bind the vao
            // - glDrawElements() !!!

           
            
           
            glClearColor(0.2, 0.2, 0.2, 1.0);
            
            glClear(GL_COLOR_BUFFER_BIT);
            

            //glViewport(0, 0, app->displaySize.x,app->displaySize.y);
           
            glBindVertexArray(app->vertexArrayObj);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);

          
        }
            break;

        default:;
    }
}

