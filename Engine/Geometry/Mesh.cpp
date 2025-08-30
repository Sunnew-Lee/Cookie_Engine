/*-------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: mesh.cpp
Purpose:
    This file contains definitions of member functions of class Mesh and Mesh creating functions.
Language: C++ 17
Platform: VS 19 / version 16.9.0 / Windows
Project: sunwoo.lee_CS300_1
Author: Sunwoo Lee / sunwoo.lee / 0055087
Creation date: 09.17.2022
--------------------------------------------------------*/
#include "Mesh.h"

Mesh::Mesh(std::string path)
{
    // parse + mapping
    OBJ_Parser(path);
    calc_BufferDatas();
    SendVertexData();
}

//todo: calc_BufferDatas(); SendVertexData(); do in Mesh::Init? Model Init=> Meshes->Init();?
//for Assimp
Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture*>& textures, float minmax[3][2], std::vector<Vec3>& fnbuffer) :
    vertexBuffer(vertices), indexBuffer(indices), textureBuffer(textures), fnBuffer(fnbuffer), aabb(AABB(Vec3(minmax[0][0], minmax[1][0], minmax[2][0]), Vec3(minmax[0][1], minmax[1][1], minmax[2][1])))
{
    vert_mapping();
    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    calc_BufferDatas();
    //uv_Sphere();  -> texture wrap
    //Setup();
    SendVertexData();
}

void Mesh::vert_mapping()
{
    float scale_factor = std::max({ aabb.Width, aabb.Height, aabb.Depth });

    MODEL_Translate = glm::translate(glm::mat4(1.0f), -aabb.center);
    MODEL_Scale = glm::scale(glm::mat4(1.0f), Vec3(2.f) / scale_factor);

    mapping = MODEL_Scale * MODEL_Translate;
}

void Mesh::calc_vert_normal()
{
    int size = static_cast<int>(vertexBuffer.size());
    for (int i{ 0 }; i < size; i++)
    {
        glm::vec3 result = glm::vec3(0.f);

        auto func = [](glm::vec3 left, glm::vec3 right) {return (glm::epsilonEqual(left.x, right.x, EPSILON) && glm::epsilonEqual(left.y, right.y, EPSILON) && glm::epsilonEqual(left.z, right.z, EPSILON)); };
        faces[i].erase(std::unique(faces[i].begin(), faces[i].end(), func), faces[i].end());

        for (glm::vec3& a : faces[i])
        {
            result += a;
        }
        vertexBuffer[i].nrm = glm::normalize(result);
    }
}

void Mesh::SendVertexData()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(3, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    /*  Copy vertex attributes to GPU */
    glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(Vertex), &vertexBuffer[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, nrm)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

    // Mesh face normal
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, fnBuffer.size() * sizeof(glm::vec3), &fnBuffer[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(0));


    // Mesh vertex normal
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, vnBuffer.size() * sizeof(glm::vec3), &vnBuffer[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(0));


    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    /*  Copy vertex indices to GPU */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(int), &indexBuffer[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::UpdateVertexData()
{
    //glBindVertexArray(VAO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ///*  Copy vertex attributes to GPU */
    //glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(Vertex), &vertexBuffer[0], GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    ///*  Copy vertex indices to GPU */
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(int), &indexBuffer[0], GL_STATIC_DRAW);

    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));

    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, nrm)));

    //glEnableVertexAttribArray(2);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
}

void Mesh::LineVertexData()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    /*  Copy vertex attributes to GPU */
    glBufferData(GL_ARRAY_BUFFER, lineposBuffer.size() * sizeof(glm::vec3), &lineposBuffer[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}

void Mesh::OBJ_Parser(const std::filesystem::path& fileName)
{
    if (fileName.extension() != ".obj") {
        throw std::runtime_error("Bad Filetype.  " + fileName.generic_string() + " not a .obj file");
    }
    std::ifstream inFile(fileName);
    if (inFile.is_open() == false) {
        throw std::runtime_error("Failed to load " + fileName.generic_string());
    }

    std::string label;
    float xyz_minmax[3][2];     //[0][0]->x min, [2][1]->z max
    bool is_init{ false };


    while (inFile.eof() == false)
    {
        inFile >> label;

        if (label == "#")
        {
            getline(inFile, label);
        }

        else if (label == "v")
        {
            Vertex vert;
            inFile >> vert.pos.x >> vert.pos.y >> vert.pos.z;

            if (is_init == false)
            {
                for (int i{ 0 }; i < 3; i++)
                {
                    for (int j{ 0 }; j < 2; j++)
                    {
                        xyz_minmax[i][j] = vert.pos[i];
                    }
                }
                is_init = true;
            }
            else
            {
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
            }

            vertexBuffer.push_back(vert);

        }

        else if (label == "f")
        {
            int face[3]{ 0 };

            for (int i{ 0 }; i < 3; i++)
            {
                std::string index[3];
                int count{ 0 };

                inFile >> label;
                for (char c : label)
                {
                    if (c == '/')
                    {
                        count++;
                        continue;
                    }
                    index[count].push_back(c);
                }
                int num = (stoi(index[0]) - 1);
                indexBuffer.push_back(num);//pos vertex index
                //vtBuffer.push_back(stoi(index[1]) - 1);//texture vertex index
                //vnBuffer.push_back(stoi(index[2]) - 1);//normal vertex index.

                face[i] = num;
            }

            glm::vec3 P = vertexBuffer[face[0]].pos;
            glm::vec3 Q = vertexBuffer[face[1]].pos;
            glm::vec3 R = vertexBuffer[face[2]].pos;

            glm::vec3 face_nrm = glm::normalize(glm::cross(Q - P, R - P));

            faces.emplace(face[0], std::vector<glm::vec3>());
            faces.emplace(face[1], std::vector<glm::vec3>());
            faces.emplace(face[2], std::vector<glm::vec3>());
            faces[face[0]].push_back(face_nrm);
            faces[face[1]].push_back(face_nrm);
            faces[face[2]].push_back(face_nrm);

            glm::vec3 tricenter = (P + Q + R) / 3.f;
            fnBuffer.push_back(tricenter);
            fnBuffer.push_back(tricenter + face_nrm);
        }

        //else if (label == "vt")
        //{
        //    Vertex vert;
        //    inFile >> vert.uv.x >> vert.uv.y;

        //    vertexBuffer.push_back(vert);
        //}

        //else if (label == "vn")
        //{
        //    Vertex vert;
        //    inFile >> vert.nrm.x >> vert.nrm.y >> vert.nrm.z;

        //    vertexBuffer.push_back(vert);
        //}

        else
        {
            //Error print;
            getline(inFile, label);
        }
    }

    //todo: pointer?
    aabb = AABB(Vec3(xyz_minmax[0][0], xyz_minmax[1][0], xyz_minmax[2][0]), Vec3(xyz_minmax[0][1], xyz_minmax[1][1], xyz_minmax[2][1]));

    vert_mapping();
    calc_vert_normal();
}

//void Mesh::setup_mesh(GLSLShader& shdr)
//{
//    shdr.Use();
//
//    /*  Lets use map */
//    //modelLoc = glGetUniformLocation(renderProg.GetHandle(), "model");
//    //viewLoc = glGetUniformLocation(renderProg.GetHandle(), "view");
//    //colorLoc = glGetUniformLocation(renderProg.GetHandle(), "color");
//    //projectionLoc = glGetUniformLocation(renderProg.GetHandle(), "projection");
//    //LightLoc = glGetUniformLocation(renderProg.GetHandle(), "lightPos");
//    //ViewPosLoc = glGetUniformLocation(renderProg.GetHandle(), "viewPos");
//
//    //SendVertexData();
//
//    /*  Bind framebuffer to 0 to render to the screen (by default already 0) */
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    /*  Initially drawing using filled mode */
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//    /*  Hidden surface removal */
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//
//    glEnable(GL_CULL_FACE);     /*  For efficiency, not drawing back-face */
//
//    shdr.UnUse();
//}

void Mesh::cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(3, VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::BuildIndexBuffer(int stacks, int slices)
{
    int p0 = 0, p1 = 0, p2 = 0;
    int p3 = 0, p4 = 0, p5 = 0;
    int stride = slices + 1;
    for (int i = 0; i < stacks; ++i)
    {
        int curr_row = i * stride;

        for (int j = 0; j < slices; ++j)
        {
            /*  You need to compute the indices for the first triangle here */
            /*  ... */
            p0 = curr_row + j;
            p1 = p0 + 1;
            p2 = p1 + stride;

            /*  Ignore degenerate triangle */
            if (!DegenerateTri(vertexBuffer[p0].pos,
                vertexBuffer[p1].pos,
                vertexBuffer[p2].pos))
            {
                /*  Add the indices for the first triangle */
                indexBuffer.push_back(p0);
                indexBuffer.push_back(p1);
                indexBuffer.push_back(p2);

                glm::vec3 P = vertexBuffer[p0].pos;
                glm::vec3 Q = vertexBuffer[p1].pos;
                glm::vec3 R = vertexBuffer[p2].pos;

                glm::vec3 face_nrm = glm::normalize(glm::cross(Q - P, R - P));
                faces.emplace(p0, std::vector<glm::vec3>());
                faces.emplace(p1, std::vector<glm::vec3>());
                faces.emplace(p2, std::vector<glm::vec3>());
                faces[p0].push_back(face_nrm);
                faces[p1].push_back(face_nrm);
                faces[p2].push_back(face_nrm);

                glm::vec3 tricenter = (P + Q + R) / 3.f;
                fnBuffer.push_back(tricenter);
                fnBuffer.push_back(tricenter + face_nrm);
            }

            /*  You need to compute the indices for the second triangle here */
            /*  ... */
            p3 = p2;
            p4 = p3 - 1;
            p5 = p0;

            /*  Ignore degenerate triangle */
            if (!DegenerateTri(vertexBuffer[p3].pos,
                vertexBuffer[p4].pos,
                vertexBuffer[p5].pos))
            {
                /*  Add the indices for the second triangle */
                indexBuffer.push_back(p3);
                indexBuffer.push_back(p4);
                indexBuffer.push_back(p5);

                glm::vec3 P = vertexBuffer[p3].pos;
                glm::vec3 Q = vertexBuffer[p4].pos;
                glm::vec3 R = vertexBuffer[p5].pos;

                glm::vec3 face_nrm = glm::normalize(glm::cross(Q - P, R - P));
                faces.emplace(p3, std::vector<glm::vec3>());
                faces.emplace(p4, std::vector<glm::vec3>());
                faces.emplace(p5, std::vector<glm::vec3>());
                faces[p3].push_back(face_nrm);
                faces[p4].push_back(face_nrm);
                faces[p5].push_back(face_nrm);

                glm::vec3 tricenter = (P + Q + R) / 3.f;
                fnBuffer.push_back(tricenter);
                fnBuffer.push_back(tricenter + face_nrm);
            }
        }
    }
}

bool Mesh::DegenerateTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
    return (glm::distance(v0, v1) < EPSILON ||
        glm::distance(v1, v2) < EPSILON ||
        glm::distance(v2, v0) < EPSILON);
}

//mapping for vn, fn lines
void Mesh::calc_BufferDatas()
{
    //calc vnBuffer
    for (Vertex& v : vertexBuffer)
    {
        vnBuffer.push_back(v.pos);
        Vec4 temp = Vec4(glm::normalize(v.nrm), 1.f) / MODEL_Scale;

        vnBuffer.push_back(v.pos + Vec3(temp) * LINE_SCALE);
    }

    //calc fnBuffer
    int size = static_cast<int>(fnBuffer.size());
    for (int i{ 1 }; i < size; i += 2)
    {
        Vec4 temp = Vec4(fnBuffer[i] - fnBuffer[i - 1], 1.f);
        fnBuffer[i] = fnBuffer[i - 1] + Vec3(temp /  MODEL_Scale) * LINE_SCALE;
    }
}

//todo: Set EBO and make index buffer for this.
void AABB::SetLines()
{
    lines.push_back(min);  //min,min,min
    lines.push_back(glm::vec3(min.x, min.y, max.z));  //min,min,max

    lines.push_back(min);  //min,min,min
    lines.push_back(glm::vec3(max.x, min.y, min.z));  //max,min,min

    lines.push_back(glm::vec3(max.x, min.y, min.z));  //max,min,min
    lines.push_back(glm::vec3(max.x, min.y, max.z));  //max,min,max

    lines.push_back(glm::vec3(max.x, min.y, min.z));  //max,min,min
    lines.push_back(glm::vec3(max.x, max.y, min.z));  //max,max,min

    lines.push_back(glm::vec3(max.x, max.y, min.z));  //max,max,min
    lines.push_back(max);  //max,max,max

    lines.push_back(glm::vec3(max.x, max.y, min.z));  //max,max,min
    lines.push_back(glm::vec3(min.x, max.y, min.z));  //min,max,min

    lines.push_back(glm::vec3(min.x, max.y, min.z));  //min,max,min
    lines.push_back(glm::vec3(min.x, max.y, max.z));  //min,max,max

    lines.push_back(glm::vec3(min.x, max.y, min.z));  //min,max,min
    lines.push_back(min);  //min,min,min

    lines.push_back(max);  //max,max,max
    lines.push_back(glm::vec3(max.x, min.y, max.z));  //max,min,max

    lines.push_back(glm::vec3(max.x, min.y, max.z));  //max,min,max
    lines.push_back(glm::vec3(min.x, min.y, max.z));  //min,min,max

    lines.push_back(glm::vec3(min.x, min.y, max.z));  //min,min,max
    lines.push_back(glm::vec3(min.x, max.y, max.z));  //min,max,max

    lines.push_back(glm::vec3(min.x, max.y, max.z));  //min,max,max
    lines.push_back(max);  //max,max,max
}
