/*-------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: scene.cpp
Purpose:
	This file contains definitions of member functions of class Scene.
Language: C++ 17
Platform: VS 19 / version 16.9.0 / Windows
Project: sunwoo.lee_CS300_1
Author: Sunwoo Lee / sunwoo.lee / 0055087
Creation date: 09.17.2022
--------------------------------------------------------*/
#include "Scene.h"

void Scene::CleanUp()
{
	for (Mesh* mesh : meshes)
	{
		mesh->cleanup();
		delete mesh;
	}
}

