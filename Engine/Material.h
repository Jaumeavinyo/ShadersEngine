#ifndef MATERIAL_H_
#define MATERIAL_H_





struct Material
{
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    glm::vec3 ambientColor;
    glm::vec3 emissiveColor;
    float shininess;
    unsigned int textureID;
};

#endif // !MATERIAL_H_
