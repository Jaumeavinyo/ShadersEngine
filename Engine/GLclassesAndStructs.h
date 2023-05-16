#pragma once
#include "VertexBufferLayout.h"
#include "Code/platform.h"


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



struct Mesh
{
    std::vector<SubMesh> submeshes;

    unsigned int VBO_handle;
    unsigned int IBO_handle;
};

struct Model {
    unsigned int meshIDx;
    std::vector<unsigned int> materialIDx;
};

struct Program
{
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