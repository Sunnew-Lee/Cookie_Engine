/*-------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: scene.h
Purpose:
	Declaration for Scene class, enum class for MeshType and ShaderType
	Handles ImGui
Language: C++ 17
Platform: VS 19 / version 16.9.0 / Windows
Project: sunwoo.lee_CS300_1
Author: Sunwoo Lee / sunwoo.lee / 0055087
Creation date: 09.17.2022
--------------------------------------------------------*/
#pragma once
#include "../Math/Math.h"
#include "../Core/EngineCore.h"

#include "../Geometry/Model.h"
#include "../Shader/glslShader.h"


//using shdr_vec = std::vector<std::pair<GLenum, std::string>>;
class Camera;

class Engine_API Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void Init(int w, int h, Camera* cam) = 0;
	virtual void mesh_setup()=0;
	inline void Load_Shaders(std::vector<shdr_vec> v)
	{
		for (shdr_vec shdr : v)
		{
			shdr_files.push_back(GLSLShader(shdr));
		}
	}

	virtual void Update(double delta_time) = 0;

	virtual void Render() = 0;

	virtual void CleanUp() = 0;
	//todo: maybe clean all objects?
	//{
	//	for (Mesh* mesh : meshes)
	//	{
	//		mesh->cleanup();
	//		delete mesh;
	//	}
	//}
	inline void Parse_Section(std::vector<std::vector<std::string>>& sections, std::string filepath, unsigned int size)
	{
		sections.resize(size);

		//pushback sections[0]~[5]
		for (int i{ 0 }; i < size; i++)
		{
			std::ifstream inFile(filepath + std::to_string(i + 1) + ".txt");
			if (inFile.is_open() == false)
			{
				//throw std::runtime_error("Failed to load Section" + std::to_string(i));
				continue;
			}

			std::string label;
			while (inFile.eof() == false)
			{
				getline(inFile, label);
				if (label.empty() || std::string(label.end() - 4, label.end()) != ".obj")
				{
					//throw std::runtime_error("Bad Filetype.  " + label + " not a .obj file");
					continue;
				}

				sections[i].push_back(label);
			}
		}
	}

protected:

	glm::mat4 view{ glm::mat4(1.0f) };
	glm::mat4 projection{ glm::mat4(1.0f) };
	//Vec3 lightPos{ -5.f, 3.f, 5.f };

	Vec3 cam_pos{ 0.f,0.f,0.f };

	//todo: set Near plane far away to make better depth buffer precision
	Vec2 Z_near_far{ 10000.f, 10000000.f };

	//todo: GLSLShader*?
	std::vector<GLSLShader> shdr_files;
	//todo: vector of Models? -> new Model()

	Camera* camera{ nullptr };
};