#pragma once
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <glm/glm.hpp>
#include <vector>
#include <list>

#include "../Geometry/AABB.h"
#include "../Core/EngineCore.h"

struct Engine_API Triangle
{
    Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
    {
        vertices.push_back(a);
        vertices.push_back(b);
        vertices.push_back(c);
    };

    std::vector<glm::vec3> vertices;
};

struct Engine_API Node
{
    Node(AABB* aabb_, int level_, std::list<Triangle> tri = std::list<Triangle>()) :aabb(aabb_), level(level_), triangles(tri)
    {
        //Gen_VAO();
    };
    Node* pChildren[8]{ nullptr };

    int level;
    AABB* aabb;
    std::list<Triangle> triangles;
    //void Gen_VAO();
};