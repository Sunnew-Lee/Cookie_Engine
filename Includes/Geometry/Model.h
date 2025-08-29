/*-------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: object.h
Purpose:
    Declaration for Object class
    Sends uniform data and draw primitives
Language: C++ 17
Platform: VS 19 / version 16.9.0 / Windows
Project: sunwoo.lee_CS300_1
Author: Sunwoo Lee / sunwoo.lee / 0055087
Creation date: 09.17.2022
--------------------------------------------------------*/
#pragma once
#include "../Core/EngineCore.h"
#include "../Math/Math.h"
#include "Mesh.h"

using shdr_vec = std::vector<std::pair<GLenum, std::string>>;


struct aiNode;
struct aiScene;
struct aiMesh;

class Engine_API Model
{
public:

    Model() {};
    void Init(Mesh* m, GLSLShader& shader);

    //Assimp reader
    void Load_Assimp(const std::string& path);

    void Update(Mat4& view, Mat4& projection, Vec3& lightpos, Vec3& lightcolor, Vec3& eye);
    void Draw(bool show_fnormal = { false }, bool show_vnormal = { false });
    //void draw_orbit();
    void CleanUp();

    inline void set_color(Vec4 color)
    { 
        if (color.r > 1.f) { color.r = 1.f; }
        if (color.g > 1.f) { color.g = 1.f; }
        if (color.b > 1.f) { color.b = 1.f; }

        selfColor = color; 
        is_updated = true;
    }

    inline void set_position(Vec3 pos)
    {
        position = pos;
        is_updated = true;
    }
    inline void set_scale(Vec3 s)
    {
        scale = s;
        is_updated = true;
    }
    inline void set_rotation(Vec3 angle)
    {
        rotation = angle;
        is_updated = true;
    }

    inline Vec3& Get_position() { return position; }
    inline Vec3& Get_scale() { return scale; }
    inline Vec3& Get_Rotation() { return rotation; }
    inline GLSLShader GetShdr_pgm() { return shdr_pgm; }
    inline void SetShdr_pgm(GLSLShader& shdr) { shdr_pgm = shdr; shdr_handle = shdr.GetHandle(); }
    inline void Set_mapping(bool do_map) { do_mapping = do_map; }

private:
    //Assimp helper functions ======================================
    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    //==============================================================

private:
    GLSLShader shdr_pgm;
    GLuint shdr_handle{ 0 };

	Vec3 position{ 0,0,0 };
	Vec3 scale{ 1,1,1 };
	Vec3 rotation{ 0,0,0 };
	Vec4 selfColor{ 0,0,0,1 };
    glm::mat4 model{ glm::mat4(1.f) };

    bool is_updated{ false };
    bool do_mapping{ false };
	
    //Mesh* mesh{ nullptr };
    std::vector<Mesh*> meshes;
    std::vector<Texture*> textures;
    std::string directory;
};