//
//
//
//#include <assimp/cimport.h>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//#include "Code/platform.h"
//#include "MeshComponent.h"
//#include <vector>
//#include <cstring>
//
//
//
//u32 LoadModel(MeshComponent*meshComponent, const char* filename)
//{
//    const aiScene* scene = aiImportFile(filename,
//        aiProcess_Triangulate |
//        aiProcess_GenSmoothNormals |
//        aiProcess_CalcTangentSpace |
//        aiProcess_JoinIdenticalVertices |
//        aiProcess_PreTransformVertices |
//        aiProcess_ImproveCacheLocality |
//        aiProcess_OptimizeMeshes |
//        aiProcess_SortByPType);
//
//    if (!scene)
//    {
//        ELOG("Error loading mesh %s: %s", filename, aiGetErrorString());
//        return UINT32_MAX;
//    }
//
//    app->meshes.push_back(Mesh{});
//    Mesh& mesh = app->meshes.back();
//    u32 meshIdx = (u32)app->meshes.size() - 1u;
//
//    app->models.push_back(Model{});
//    Model& model = app->models.back();
//    model.meshIdx = meshIdx;
//    u32 modelIdx = (u32)app->models.size() - 1u;
//
//    String directory = GetDirectoryPart(MakeString(filename));
//
//    // Create a list of materials
//    u32 baseMeshMaterialIndex = (u32)app->materials.size();
//    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
//    {
//        app->materials.push_back(Material{});
//        Material& material = app->materials.back();
//        ProcessAssimpMaterial(app, scene->mMaterials[i], material, directory);
//    }
//
//    ProcessAssimpNode(scene, scene->mRootNode, &mesh, baseMeshMaterialIndex, model.materialIdx);
//
//    aiReleaseImport(scene);
//
//    u32 vertexBufferSize = 0;
//    u32 indexBufferSize = 0;
//
//    for (u32 i = 0; i < mesh.submeshes.size(); ++i)
//    {
//        vertexBufferSize += mesh.submeshes[i].vertices.size() * sizeof(float);
//        indexBufferSize += mesh.submeshes[i].indices.size() * sizeof(u32);
//    }
//
//    glGenBuffers(1, &mesh.vertexBufferHandle);
//    glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferHandle);
//    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_STATIC_DRAW);
//
//    glGenBuffers(1, &mesh.indexBufferHandle);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferHandle);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, NULL, GL_STATIC_DRAW);
//
//    u32 indicesOffset = 0;
//    u32 verticesOffset = 0;
//
//    for (u32 i = 0; i < mesh.submeshes.size(); ++i)
//    {
//        const void* verticesData = mesh.submeshes[i].vertices.data();
//        const u32   verticesSize = mesh.submeshes[i].vertices.size() * sizeof(float);
//        glBufferSubData(GL_ARRAY_BUFFER, verticesOffset, verticesSize, verticesData);
//        mesh.submeshes[i].vertexOffset = verticesOffset;
//        verticesOffset += verticesSize;
//
//        const void* indicesData = mesh.submeshes[i].indices.data();
//        const u32   indicesSize = mesh.submeshes[i].indices.size() * sizeof(u32);
//        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesOffset, indicesSize, indicesData);
//        mesh.submeshes[i].indexOffset = indicesOffset;
//        indicesOffset += indicesSize;
//    }
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    return modelIdx;
//}
//
