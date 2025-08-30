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

//todo: hmm... not using Texture for now.
struct Engine_API Texture {
    unsigned int id{ 0 };
    std::string type;
    std::string path;
};

struct AABB
{

    AABB() {};
    AABB(glm::vec3 min_, glm::vec3 max_) : min(min_), max(max_)
    {
        center = Vec3((min[0] + max[0]), (min[1] + max[1]), (min[2] + max[2])) * 0.5f;
        Width = max[0] - min[0];
        Height = max[1] - min[1];
        Depth = max[2] - min[2];
    };
    void SetLines();


    //add lines for AABB here
    std::vector<glm::vec3> lines = std::vector<glm::vec3>();

    //x,y,z min value
    Vec3 min = Vec3(0.f);
    //x,y,z max value
    Vec3 max = Vec3(0.f);

    Vec3 center = Vec3(0.f);

    float Width; 
    float Height;
    float Depth; 
};

struct Triangle
{
    Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
    {
        vertices.push_back(a);
        vertices.push_back(b);
        vertices.push_back(c);
    };

    std::vector<glm::vec3> vertices;
};

struct OctreeNode
{
    OctreeNode(AABB* aabb_, int level_, std::list<Triangle> tri = std::list<Triangle>()) :aabb(aabb_), level(level_), triangles(tri)
    {
        Gen_VAO();
    };
    int level;
    OctreeNode* pChildren[8]{ nullptr };
    AABB* aabb;
    std::list<Triangle> triangles;
    GLuint TREE_VAO = 0;
    GLuint TREE_VBO = 0;
    void Gen_VAO();
};

struct Engine_API Mesh
{
    Mesh() {};

    //for procedural mesh creation
    Mesh(std::string path);

    //for Assimp return. Make Mesh out of all the data Assimp parsed.
    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture*>& textures, float minmax[3][2], std::vector<Vec3>& fnbuffer);

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
    void vert_mapping();

    // make vert normals out of face normals.
    void calc_vert_normal();
    //===============================================================

    //void setup_mesh(GLSLShader& shdr);

    void cleanup();

    void BuildIndexBuffer(int stacks, int slices);
    bool DegenerateTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

    void calc_BufferDatas();
    void LineVertexData();


    glm::mat4 Get_mapping(void)
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
    }

    /*  Storing the actual vertex/index data */
    std::vector<Vertex> vertexBuffer;
    std::vector<unsigned int> indexBuffer;  // note that we start from 0 (GLint)
    std::vector<Texture*> textureBuffer; // not using it for now

    std::map<int, std::vector<glm::vec3>> faces;

    std::vector<glm::vec3> fnBuffer;
    std::vector<glm::vec3> vnBuffer;
    std::vector<glm::vec3> lineposBuffer;

    static constexpr float  EPSILON = 0.00001f;
    static constexpr float  LINE_SCALE = 0.125f;

    GLuint VBO[3] = { 0 };
    GLuint VAO = 0;
    GLuint EBO = 0;

    glm::mat4 mapping{ glm::mat4(1.f) };
    glm::mat4 MODEL_Translate{ glm::mat4(1.f) };
    glm::mat4 MODEL_Scale{ glm::mat4(1.f) };

    AABB aabb;
};





//void Object::Octree_Setup()
//{
//    glCreateVertexArrays(1, &Oct_VAO);
//    glBindVertexArray(Oct_VAO);
//
//    glGenBuffers(1, &Oct_VBO);
//    glBindVertexArray(0);
//
//    for (auto m : meshes)
//    {
//        int size = m.vertexBuffer.size();
//        for (int i{ 0 }; i < size; i += 3)
//        {
//            triangles.push_back(Triangle(m.vertexBuffer[i].pos, m.vertexBuffer[i + 1].pos, m.vertexBuffer[i + 2].pos));
//        }
//    }
//
//    Octree_root = new OctreeNode(BottomUp_AABB_root->bv, 1, triangles);
//    Build_Octree(Octree_root);
//}
//
//#include <stack>
//void Object::DivideOrAdd(OctreeNode* node, Triangle tri)
//{
//
//    for (int i = 0; i < 8; ++i)
//    {
//        if (Tri_check(tri, node->pChildren[i]->aabb))
//        {
//            node->pChildren[i]->triangles.push_back(tri);
//            return;
//        }
//    }
//
//    std::stack<Triangle> temp_triangles;
//    temp_triangles.push(tri);
//
//    for (int i = 0; i < 3; i++) // x,y,z
//    {
//        std::vector<Triangle> temp;
//        while (!temp_triangles.empty())
//        {
//            auto triangle = temp_triangles.top();
//
//            auto newTriangleInfo = IntersectRayPlane(node, i, triangle);
//            bool isGenerate = newTriangleInfo.first;
//
//            if (isGenerate)
//            {
//                auto newTriangles = newTriangleInfo.second;
//                temp.insert(temp.end(), newTriangles.begin(), newTriangles.end());
//            }
//            else
//            {
//                temp.push_back(triangle);
//            }
//
//            temp_triangles.pop();
//        }
//
//        for (auto newTriangle : temp)
//            temp_triangles.push(newTriangle);
//    }
//
//
//    while (!temp_triangles.empty())
//    {
//        auto newTriangle = temp_triangles.top();
//        for (int i = 0; i < 8; i++)
//        {
//            if (Tri_check(newTriangle, node->pChildren[i]->aabb))
//            {
//                node->pChildren[i]->triangles.push_back(newTriangle);
//            }
//        }
//        temp_triangles.pop();
//    }
//}
//
//bool Object::Tri_check(const Triangle& tri, AABB* aabb)
//{
//    for (int i = 0; i < 3; i++) {
//        if (tri.vertices[i].x < aabb->min.x || tri.vertices[i].x > aabb->max.x ||
//            tri.vertices[i].y < aabb->min.y || tri.vertices[i].y > aabb->max.y ||
//            tri.vertices[i].z < aabb->min.z || tri.vertices[i].z > aabb->max.z)
//        {
//            return false;
//        }
//    }
//
//
//    return true;
//}
//
//#include <glm/gtx/intersect.hpp>
//
//std::pair<bool, std::vector<Triangle>> Object::IntersectRayPlane(OctreeNode* node, int axisIndex, Triangle v)
//{
//    std::vector<Triangle> resultT;
//    glm::vec3 normal;
//
//    switch (axisIndex)
//    {
//    case 0: //X
//        normal = glm::vec3(1, 0, 0);
//        break;
//
//    case 1: //Y
//        normal = glm::vec3(0, 1, 0);
//        break;
//
//    case 2: //Z
//        normal = glm::vec3(0, 0, 1);
//        break;
//    }
//    std::vector<glm::vec3> front;
//    std::vector<glm::vec3> back;
//    std::vector<glm::vec3> inside;
//
//    for (int i = 0; i < 3; ++i)
//    {
//        if (node->aabb->center[axisIndex] < v.vertices[i][axisIndex])
//            front.push_back(v.vertices[i]);
//        else if (node->aabb->center[axisIndex] > v.vertices[i][axisIndex])
//            back.push_back(v.vertices[i]);
//        else
//            inside.push_back(v.vertices[i]);
//    }
//
//    if (back.empty() || front.empty())
//        return { false, resultT };
//
//    if (!inside.empty())
//    {
//        glm::vec3 v1 = front[0];
//        glm::vec3 v2 = back[0];
//        glm::vec3 dir = glm::normalize(v2 - v1);
//        float distance = 0.0f;
//
//        bool intersect = glm::intersectRayPlane(v1, dir, node->aabb->center, normal, distance);
//
//        if (!intersect)
//            return { false, resultT };
//
//        glm::vec3 insideV = inside[0];
//        glm::vec3 i1 = v1 + distance * dir;
//
//        Triangle t1{ v1, i1, insideV };
//        Triangle t2{ i1, v2, insideV };
//
//        resultT.insert(resultT.end(), { t1,t2 });
//    }
//    else
//    {
//        glm::vec3 v1;
//        glm::vec3 v2;
//        glm::vec3 v3;
//        if (back.size() < front.size())
//        {
//            v1 = back[0];
//            v2 = front[0];
//            v3 = front[1];
//        }
//        else
//        {
//            v1 = front[0];
//            v2 = back[0];
//            v3 = back[1];
//        }
//
//        glm::vec3 dir = glm::normalize(v2 - v1);
//        float distance;
//
//        bool intersect = glm::intersectRayPlane(v1, dir, node->aabb->center, normal, distance);
//        if (!intersect)
//            return { false, resultT };
//        glm::vec3 i1 = v1 + distance * dir;
//
//        dir = glm::normalize(v3 - v1);
//        intersect = glm::intersectRayPlane(v1, dir, node->aabb->center, normal, distance);
//        if (!intersect)
//            return { false, resultT };
//        glm::vec3 i2 = v1 + distance * dir;
//
//        Triangle t1{ v1, i2, i1 };
//        Triangle t2{ i1, i2, v2 };
//        Triangle t3{ i2, v3, v2 };
//        resultT.insert(resultT.end(), { t1,t2,t3 });
//    }
//
//    return { true, resultT };
//}