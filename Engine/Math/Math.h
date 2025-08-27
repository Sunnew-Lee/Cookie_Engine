#pragma once
#include <glm/glm.hpp>

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;

struct Vertex
{
    Vertex(glm::vec3 pos, glm::vec3 nrm, glm::vec2 uv) : pos(pos), nrm(nrm), uv(uv) {}

    Vertex() : pos({ 0,0,0 }), nrm({ 0,0,0 }), uv({ 0,0 }) {}

    glm::vec3 pos, nrm;
    glm::vec2 uv;

};
