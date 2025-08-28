#pragma once
#include "../../Engine/Scene/Scene.h"

enum class MeshType
{
	BUNNY,
	SPHERE4,
	CUBE2,
	SPHERE,
	SPHERE_MOD,
	//CUP,
	//LUCY,
	//STARWARS,
	PROCEDURAL_SPHERE,
	ORBIT,
	COUNT
};

enum class ShdrType
{
	MODEL_PHONG,
	LINE,
	COUNT
};

class Demo : public Scene
{
public:
	Demo() {};
	virtual ~Demo() override {};

	virtual void Init(int w, int h) override;
	virtual void mesh_setup() override;
	virtual void shdr_file_setup() override;

	virtual void Update(double delta_time) override;

	virtual void Render() override;

	virtual void CleanUp() override;
private:

	Vec3 lightPos{ -5.f, 3.f, 5.f };

	static constexpr int SATELLITE_NUM{ 8 };
	static constexpr int ORBIT_VERT{ 100 };
	static constexpr float RAD = 3.f;
	static constexpr float ANGLE = 360.f / SATELLITE_NUM;

	Model CenterOBJ;
	Model Satellites[SATELLITE_NUM];
	Model Orbit;

	bool show_fnormal{ false };
	bool show_vnormal{ false };

	static constexpr int ITEM_NUM = 5;
	const char* mesh_items[ITEM_NUM]
	{
		"BUNNY","SPHERE4","CUBE2","SPHERE","SPHERE_MOD"
	};
	int selected_item{ 0 };
};

Mesh* CreateSphere(int stacks, int slices);
Mesh* CreateOrbit(float radius, GLuint vert);