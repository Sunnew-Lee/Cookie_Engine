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

	virtual void Init(int w, int h, Camera* cam) override;
	virtual void mesh_setup() override;
	virtual void shdr_file_setup() override;

	virtual void Update(double delta_time) override;

	virtual void Render() override;

	virtual void CleanUp() override;

private:
	void Update_ImGui();
private:
	Vec3 lightPos{ -5.f, 3.f, 5.f };
	Vec3 lightColor{ 0.08f, 0.34f, 0.97f };

	Model CenterOBJ;

	bool show_fnormal{ false };
	bool show_vnormal{ false };

	static constexpr int ITEM_NUM = 5;
	const char* mesh_items[ITEM_NUM]
	{
		"BUNNY","SPHERE4","CUBE2","SPHERE","SPHERE_MOD"
	};
	int selected_item{ 0 };
};

//Mesh* CreateSphere(int stacks, int slices);
//Mesh* CreateOrbit(float radius, GLuint vert);