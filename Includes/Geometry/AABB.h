#pragma once
//#include <GL/glew.h> // for access to OpenGL API declarations 
#include "../Core/EngineCore.h"

#include <glm/glm.hpp>
#include <vector>

struct Engine_API AABB
{
    AABB() {};
    AABB(glm::vec3 min_, glm::vec3 max_) : min(min_), max(max_)
    {
        center = glm::vec3((min[0] + max[0]), (min[1] + max[1]), (min[2] + max[2])) * 0.5f;
        Width = max[0] - min[0];
        Height = max[1] - min[1];
        Depth = max[2] - min[2];
    };
    void SetLines();


    //add lines for AABB here
    std::vector<glm::vec3> lines = std::vector<glm::vec3>();

    //x,y,z min value
    glm::vec3 min = glm::vec3(0.f);
    //x,y,z max value
    glm::vec3 max = glm::vec3(0.f);

    glm::vec3 center = glm::vec3(0.f);

    float Width;
    float Height;
    float Depth;
};