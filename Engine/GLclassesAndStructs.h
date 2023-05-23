#pragma once
#include "VertexBufferLayout.h"
#include "VertexShaderLayout.h"
#include "Code/platform.h"


typedef glm::vec2  vec2;
typedef glm::vec3  vec3;
typedef glm::vec4  vec4;
typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;

enum LightType {
    LightType_Directional,
    LightType_Point
};


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

struct Material
{
    std::string name;

    vec3 albedo;
    vec3 emissive;
    float smoothness;
    unsigned int albedoTextureIDx;
    unsigned int emissivetextureIDx;
    unsigned int specularTextureIDx;
    unsigned int normalsTextureIDx;
    unsigned int bumpTextureIDx;
};

struct VAO {
    unsigned int handle;
    unsigned int programHandle;
};

struct SubMesh {
    VertexBufferLayout VBLayout;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int vertexOffset;
    unsigned int indexOffset;

    std::vector<VAO> vaos;
};

struct Light {
    LightType type;
    vec3 color;
    vec3 position;
    vec3 direction;
    float range;
};

struct Mesh
{
    std::vector<SubMesh> submeshes;

    unsigned int VBO_handle;
    unsigned int IBO_handle;
};

struct Model {
    unsigned int meshIDx;
    std::vector<unsigned int> materialIDx;

    glm::mat4 localTransform;
};

struct Camera {

    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;

    glm::vec3 cameraDirection;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;

    glm::mat4 lookAtMat;

    //to pass from local to world coords
    glm::mat4 modelTransform;

    //transform world coordinates to view space
    glm::mat4 viewTransform;

    //converts coordinates within this specified range to normalized device coordinates (-1.0, 1.0) (not directly, a step called Perspective Division sits in between).
    //projects 3D coordinates to the easy-to-map-to-2D normalized device coordinates. = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    glm::mat4 projectionTransform;
};

struct Program
{
    VertexShaderLayout VSLayout;
    unsigned int                handle;
    std::string                 filepath;
    std::string                 programName;
    unsigned long long int      lastWriteTimestamp;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
};


enum Mode
{
    Mode_TexturedQuad,
    Mode_Count
};