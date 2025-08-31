#pragma once
#include "Node.h"
#include "../Geometry/Mesh.h"
#include "../Core/EngineCore.h"

struct Engine_API Octree
{
	Octree() {};

	void Init(std::vector<Mesh*>& meshes ,AABB* aabb);
	void Build_Octree();
    void CleanUp();

	void Start_Recursion(Node* node);
	void DivideOrAdd(Node* node, Triangle tri);
	bool Tri_check(const Triangle& tri, AABB* aabb);
	std::pair<bool, std::vector<Triangle>> IntersectRayPlane(Node* node, int axisIndex, Triangle v);

	void Render_PreorderTraversal(GLSLShader& shader, Node* root);

	Node* octreeRoot{ nullptr };
    //delete Node's AABB + Node itself -> delete octreeRoot
    void Clean_PreorderTraversal(Node* root);

	GLuint TREE_VAO = 0;
	GLuint TREE_VBO = 0;

    Vec4 colors[21] = {
        Vec4(0.f),                   // 0: NULL(no level 0)
    Vec4(1.0f, 0.0f, 0.0f, 1.0f),    // 1: Red (빨강)
    Vec4(0.0f, 1.0f, 0.0f, 1.0f),    // 2: Green (초록)
    Vec4(0.0f, 0.0f, 1.0f, 1.0f),    // 3: Blue (파랑)
    Vec4(1.0f, 1.0f, 0.0f, 1.0f),    // 4: Yellow (노랑)
    Vec4(1.0f, 0.0f, 1.0f, 1.0f),    // 5: Magenta (마젠타)
    Vec4(0.0f, 1.0f, 1.0f, 1.0f),    // 6: Cyan (시안)
    Vec4(1.0f, 0.5f, 0.0f, 1.0f),    // 7: Orange (주황)
    Vec4(0.5f, 0.0f, 1.0f, 1.0f),    // 8: Purple (보라)
    Vec4(1.0f, 0.75f, 0.8f, 1.0f),   // 9: Pink (분홍)
    Vec4(0.6f, 0.4f, 0.2f, 1.0f),    // 10: Brown (갈색)
    Vec4(0.5f, 1.0f, 0.83f, 1.0f),   // 11: Aquamarine (아쿠아마린)
    Vec4(0.0f, 0.5f, 0.5f, 1.0f),    // 12: Teal (청록)
    Vec4(0.5f, 0.5f, 0.5f, 1.0f),    // 13: Gray (회색)
    Vec4(1.0f, 1.0f, 1.0f, 1.0f),    // 14: White (흰색)
    Vec4(0.5f, 0.0f, 0.0f, 1.0f),    // 15: Maroon (적갈색)
    Vec4(0.0f, 0.0f, 0.5f, 1.0f),    // 16: Navy (남색)
    Vec4(0.5f, 0.5f, 0.0f, 1.0f),    // 17: Olive (올리브)
    Vec4(0.75f, 1.0f, 0.0f, 1.0f),   // 18: Lime (라임)
    Vec4(1.0f, 0.84f, 0.0f, 1.0f),   // 19: Gold (금색)
    Vec4(0.96f, 0.26f, 0.21f, 1.0f)  // 20: Tomato (토마토)
    };

    bool show_AABB{ false };
    bool* show_AABB_Level{nullptr};

    int criteria_level{ 0 };
    static constexpr int criteria[4]{ 2500,1000,500,300 };
};

