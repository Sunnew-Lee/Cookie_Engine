/*-------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: object.cpp
Purpose:
    This file contains definitions of member functions of class Object.
Language: C++ 17
Platform: VS 19 / version 16.9.0 / Windows
Project: sunwoo.lee_CS300_1
Author: Sunwoo Lee / sunwoo.lee / 0055087
Creation date: 09.17.2022
--------------------------------------------------------*/
#include "Model.h"

//add assimp func
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Model::Init(Mesh* m, GLSLShader& shader)
{
    //mesh = m;
    meshes.push_back(m);
    shdr_pgm = shader;
    shdr_handle = shader.GetHandle();
    //shdr_pgm.CompileLinkValidate(shdr_files);
    //if (GL_FALSE == shdr_pgm.IsLinked()) {
    //    std::cout << "Unable to compile/link/validate shader programs" << "\n";
    //    std::cout << shdr_pgm.GetLog() << std::endl;
    //    std::exit(EXIT_FAILURE);
    //}
    //shdr_handle = shdr_pgm.GetHandle();

    meshes[0]->setup_mesh(shdr_pgm);
}

//todo: all Section text needs to include "../3Dmodels/"
void Model::Load_Assimp(const std::string& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);

    //todo: nessesary?? + need to set shdr before Load_Assimp!!!!!!
    //for (Mesh* m : meshes)
    //{
    //    m->setup_mesh(shdr_pgm);
    //}
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    //std::vector<Texture> textures;
    float xyz_minmax[3][2]{};
    std::vector<glm::vec3> fnBuffer;

    // walk through each of the mesh's vertices

    for (int i{ 0 }; i < 2; i++)
    {
        xyz_minmax[0][i] = mesh->mVertices[i].x;
        xyz_minmax[1][i] = mesh->mVertices[i].y;
        xyz_minmax[2][i] = mesh->mVertices[i].z;
    }

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vert;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vert.pos = vector;
        for (int i{ 0 }; i < 3; i++)
        {
            if (xyz_minmax[i][0] > vert.pos[i])
            {
                xyz_minmax[i][0] = vert.pos[i];
            }
            else if (xyz_minmax[i][1] < vert.pos[i])
            {
                xyz_minmax[i][1] = vert.pos[i];
            }
        }
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            //mesh->mFaces[0].mIndices[0]
            vert.nrm = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            //glm::vec2 vec;
            //// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            //// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            //vec.x = mesh->mTextureCoords[0][i].x;
            //vec.y = mesh->mTextureCoords[0][i].y;
            //vertex.uv = vec;
            //// tangent
            //vector.x = mesh->mTangents[i].x;
            //vector.y = mesh->mTangents[i].y;
            //vector.z = mesh->mTangents[i].z;
            //vertex.Tangent = vector;
            //// bitangent
            //vector.x = mesh->mBitangents[i].x;
            //vector.y = mesh->mBitangents[i].y;
            //vector.z = mesh->mBitangents[i].z;
            //vertex.Bitangent = vector;
        }
        else
        {
            vert.uv = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vert);
    }

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        Vec3 index;
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
            index[j] = face.mIndices[j];
        }
        glm::vec3 P = vertices[index[0]].pos;
        glm::vec3 Q = vertices[index[1]].pos;
        glm::vec3 R = vertices[index[2]].pos;

        glm::vec3 face_nrm = glm::normalize(glm::cross(Q - P, R - P));

        glm::vec3 tricenter = (P + Q + R) / 3.f;
        fnBuffer.push_back(tricenter);
        fnBuffer.push_back(tricenter + face_nrm);

    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    //// 1. diffuse maps
    //vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    //textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    //// 2. specular maps
    //vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    //textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //// 3. normal maps
    //std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    //textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    //// 4. height maps
    //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    //// return a mesh object created from the extracted mesh data
    //return Mesh(vertices, indices, textures);

    return new Mesh(vertices, indices, textures, xyz_minmax, fnBuffer);
}


//todo: model gets shdr as param?
void Model::Update(Mat4& view, Mat4& projection, Vec3& lightpos, Vec3& lightcolor, Vec3& eye)
{
    shdr_pgm.Use();

    if (is_updated == true)
    {
        //todo: ImGui to change model pos, rot, scale?
        model = glm::translate(glm::mat4(1.f), position);
        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        is_updated = false;
    }

    glUniformMatrix4fv(glGetUniformLocation(shdr_handle, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shdr_handle, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shdr_handle, "view"), 1, GL_FALSE, glm::value_ptr(view));

    //todo: dont use for Power Plant
    if(do_mapping)
        glUniformMatrix4fv(glGetUniformLocation(shdr_handle, "mapping"), 1, GL_FALSE, glm::value_ptr(meshes[0]->Get_mapping()));

    glUniform4fv(glGetUniformLocation(shdr_handle, "color"), 1, glm::value_ptr(selfColor));

    glUniform3fv(glGetUniformLocation(shdr_handle, "light_color"), 1, glm::value_ptr(lightcolor));
    glUniform3fv(glGetUniformLocation(shdr_handle, "light_pos"), 1, glm::value_ptr(lightpos));
    glUniform3fv(glGetUniformLocation(shdr_handle, "viewPos"), 1, glm::value_ptr(eye));

    shdr_pgm.UnUse();
}


void Model::Draw(bool show_fnormal, bool show_vnormal)
{
    shdr_pgm.Use();

    for (auto mesh : meshes)
    {
        glBindVertexArray(mesh->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, nrm)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->indexBuffer.size()), GL_UNSIGNED_INT, nullptr);

        if (show_fnormal == true)
        {
            // enable face normal
            glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO[1]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)(0));
            glUniform4fv(glGetUniformLocation(shdr_handle, "color"), 1, glm::value_ptr(glm::vec4(0.2f, 0.5f, 0.2f, -1.f)));   // green for face normal
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mesh->fnBuffer.size()));
        }

        if (show_vnormal == true)
        {
            // enable vertex normal
            glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO[2]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)(0));
            glUniform4fv(glGetUniformLocation(shdr_handle, "color"), 1, glm::value_ptr(glm::vec4(0.4f, 0.3f, 0.6f, -1.f)));  // blue for vertex normal
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mesh->vnBuffer.size()));
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }    

    shdr_pgm.UnUse();
}

//void Model::draw_orbit()
//{
//    shdr_pgm.Use();
//
//    glBindVertexArray(mesh->VAO);
//
//    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mesh->lineposBuffer.size()));
//
//    glBindVertexArray(0);
//
//    shdr_pgm.UnUse();
//}

void Model::CleanUp()
{
    shdr_pgm.DeleteShaderProgram();
}
