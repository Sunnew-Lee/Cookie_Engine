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
	void shdr_file_setup();

	virtual void Update(double delta_time) override;

	virtual void Render() override;

	virtual void CleanUp() override;

private:
	void Update_ImGui();
	//void Parse_Section();
	std::vector<std::vector<std::string>> sections;
private:
	Vec3 lightPos{ -5.f, 3.f, 5.f };
	//Vec3 lightColor{ 0.08f, 0.34f, 0.97f };
	Vec3 lightColor{ 1.f, 1.f, 1.f };

	Model PowerPlant;

	bool show_fnormal{ false };
	bool show_vnormal{ false };

	bool show_Octree{ false };

	bool show_AABB{ false };
	bool show_AABB_Level[8]{ false,false,false,false,false,false,false,false };

	static constexpr int CRITERIA_NUM = 4;
	const char* criteria_items[CRITERIA_NUM]
	{
		"2500","1000","500","300"
	};
	int selected_criteria{ 0 };
};

//Mesh* CreateSphere(int stacks, int slices);
//Mesh* CreateOrbit(float radius, GLuint vert);