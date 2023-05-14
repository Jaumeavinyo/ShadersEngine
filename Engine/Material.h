#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Code/engine.h"
#include "Texture.h"


struct Material
{
    const char* name;
    glm::vec3 albedo;
    Texture albedoTextureIdx;

    
    glm::vec3 specular;
    Texture specularTextureIdx;

    glm::vec3 ambient;

    glm::vec3 emissive;
    Texture emissiveTextureIdx;

    glm::vec3 diffuse;

    Texture normalsTextureIdx;
    Texture bumpTextureIdx;

    float smoothness;
    float shininess;
    unsigned int textureID;
};

#endif // !MATERIAL_H_
