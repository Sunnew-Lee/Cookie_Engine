/*-------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: mesh.h
Purpose:
    Declaration for Mesh struct and Vertex struct
    Setup VAO, VBO and EBO
    Loads .obj files and calculates to translate to origin and map to [-1,1]
    Generate and bind buffers
Language: C++ 17
Platform: VS 19 / version 16.9.0 / Windows
Project: sunwoo.lee_CS300_1
Author: Sunwoo Lee / sunwoo.lee / 0055087
Creation date: 09.17.2022
--------------------------------------------------------*/
#pragma once
#include <filesystem>

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

#include "../Math/Math.h"
#include "../Core/EngineCore.h"

#include <map>
#include "../Shader/glslShader.h"

struct Engine_API Mesh
{
    Mesh() {};
    Mesh(std::string path);

    void SendVertexData();
    void UpdateVertexData();

    /*
    * These three functions for Parsing data out of .obj files.
    * Do not use if using Assimp.
    */
    //===============================================================
    // read vertex data from .obj file and make face normals.
    void OBJ_Parser(const std::filesystem::path& fileName);

    // set Model Translate, Scale, mapping from AABB data
    //void vert_mapping(float (*xyz_minmax)[2]);

    // make vert normals out of face normals.
    void calc_vert_normal();
    //===============================================================

    void setup_mesh(GLSLShader& shdr);

    void cleanup();

    void BuildIndexBuffer(int stacks, int slices);
    bool DegenerateTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

    void calc_BufferDatas();
    void LineVertexData();


    /*glm::mat4 Get_mapping(void)
    {
        return mapping;
    }
    glm::mat4 Get_OBJ_Translate(void)
    {
        return MODEL_Translate;
    }
    glm::mat4 Get_OBJ_Scale(void)
    {
        return MODEL_Scale;
    }*/

    /*  Storing the actual vertex/index data */
    std::vector<Vertex> vertexBuffer;
    std::vector<GLint> indexBuffer;  // note that we start from 0

    std::map<int, std::vector<glm::vec3>> faces;

    std::vector<glm::vec3> fnBuffer;
    std::vector<glm::vec3> vnBuffer;
    std::vector<glm::vec3> lineposBuffer;

    static constexpr float  EPSILON = 0.00001f;
    static constexpr float  LINE_SCALE = 0.125f;

    GLuint VBO[3] = { 0 };
    GLuint VAO = 0;
    GLuint EBO = 0;

    //glm::mat4 mapping{ glm::mat4(1.f) };
    /*glm::mat4 MODEL_Translate{ glm::mat4(1.f) };
    glm::mat4 MODEL_Scale{ glm::mat4(1.f) };*/
};