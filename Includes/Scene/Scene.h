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


//using shdr_vec = std::vector<std::pair<GLenum, std::string>>;
class Camera;

class Engine_API Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void Init(int w, int h, Camera* cam) = 0;
	virtual void mesh_setup()=0;
	virtual void shdr_file_setup()=0;

	virtual void Update(double delta_time) = 0;

	virtual void Render() = 0;

	inline virtual void CleanUp()
	{
		for (Mesh* mesh : meshes)
		{
			mesh->cleanup();
			delete mesh;
		}
	}

protected:

	glm::mat4 view{ glm::mat4(1.0f) };
	glm::mat4 projection{ glm::mat4(1.0f) };
	//Vec3 lightPos{ -5.f, 3.f, 5.f };

	Vec3 cam_pos{ 0.f,0.f,0.f };
	Vec2 Z_near_far{ 0.1f, 100.f };

	std::vector<shdr_vec> shdr_files;
	std::vector<Mesh*> meshes;

	Camera* camera{ nullptr };
};