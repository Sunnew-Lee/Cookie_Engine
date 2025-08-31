#include "Octree.h"

#include <stack>
#include <glm/gtx/intersect.hpp>

// set VAO,VBO, triangles from whole mesh, octreeRoot.
void Octree::Init(std::vector<Mesh*>& meshes, AABB* aabb)
{
    glCreateVertexArrays(1, &TREE_VAO);
    glBindVertexArray(TREE_VAO);

    glGenBuffers(1, &TREE_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, TREE_VBO);

    // 4. VAO에 VBO 데이터의 해석 방법을 설정합니다. (Dynamic 버전과 동일)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    //After all meshes for this model is setup!!
    std::list<Triangle> triangles;
    for (auto m : meshes)
    {
        int size = m->vertexBuffer.size();
        for (int i{ 0 }; i < size; i += 3)
        {
            triangles.push_back(Triangle(m->vertexBuffer[i].pos, m->vertexBuffer[i + 1].pos, m->vertexBuffer[i + 2].pos));
        }
    }

    octreeRoot = new Node(aabb, 1, triangles);
}

void Octree::Build_Octree()
{
    Start_Recursion(octreeRoot);
}

void Octree::CleanUp()
{
    glDeleteVertexArrays(1, &TREE_VAO);
    glDeleteBuffers(1, &TREE_VBO);
    Clean_PreorderTraversal(octreeRoot);
    //SafeDelete(octreeRoot);
}

void Octree::Start_Recursion(Node* node)
{
    if (node->triangles.size() <= criteria[criteria_level])
    {
        return;
    }

    //divide this->AABB in to 8 pieces and assign to Children
    glm::vec3 min = node->aabb->min;
    glm::vec3 max = node->aabb->max;
    glm::vec3 center = node->aabb->center;

    AABB* aabbs[8];
    aabbs[0] = new AABB(min, center);
    aabbs[1] = new AABB(glm::vec3(center.x, min.y, min.z), glm::vec3(max.x, center.y, center.z));
    aabbs[2] = new AABB(glm::vec3(center.x, min.y, center.z), glm::vec3(max.x, center.y, max.z));
    aabbs[3] = new AABB(glm::vec3(min.x, min.y, center.z), glm::vec3(center.x, center.y, max.z));
    aabbs[4] = new AABB(glm::vec3(min.x, center.y, min.z), glm::vec3(center.x, max.y, center.z));
    aabbs[5] = new AABB(glm::vec3(center.x, center.y, min.z), glm::vec3(max.x, max.y, center.z));
    aabbs[6] = new AABB(center, max);
    aabbs[7] = new AABB(glm::vec3(min.x, center.y, center.z), glm::vec3(center.x, max.y, max.z));

    for (int i{ 0 }; i < 8; ++i)
    {
        aabbs[i]->SetLines();
        node->pChildren[i] = new Node(aabbs[i], node->level + 1);
    }

    /*AABB* aabb0 = new AABB(min, center);
    aabb0->SetLines();
    node->pChildren[0] = new Node(aabb0, node->level + 1);

    AABB* aabb1 = new AABB(glm::vec3(center.x, min.y, min.z), glm::vec3(max.x, center.y, center.z));
    aabb1->SetLines();
    node->pChildren[1] = new Node(aabb1, node->level + 1);

    AABB* aabb2 = new AABB(glm::vec3(center.x, min.y, center.z), glm::vec3(max.x, center.y, max.z));
    aabb2->SetLines();
    node->pChildren[2] = new Node(aabb2, node->level + 1);

    AABB* aabb3 = new AABB(glm::vec3(min.x, min.y, center.z), glm::vec3(center.x, center.y, max.z));
    aabb3->SetLines();
    node->pChildren[3] = new Node(aabb3, node->level + 1);

    AABB* aabb4 = new AABB(glm::vec3(min.x, center.y, min.z), glm::vec3(center.x, max.y, center.z));
    aabb4->SetLines();
    node->pChildren[4] = new Node(aabb4, node->level + 1);

    AABB* aabb5 = new AABB(glm::vec3(center.x, center.y, min.z), glm::vec3(max.x, max.y, center.z));
    aabb5->SetLines();
    node->pChildren[5] = new Node(aabb5, node->level + 1);

    AABB* aabb6 = new AABB(center, max);
    aabb6->SetLines();
    node->pChildren[6] = new Node(aabb6, node->level + 1);

    AABB* aabb7 = new AABB(glm::vec3(min.x, center.y, center.z), glm::vec3(center.x, max.y, max.z));
    aabb7->SetLines();
    node->pChildren[7] = new Node(aabb7, node->level + 1);*/

    // for each triangle in this node
    for (const auto& tri : node->triangles)
    {
        // check which children this triangle needs to go
        DivideOrAdd(node, tri);
    }

    node->triangles.clear();

    for (int i = 0; i < 8; i++)
    {
        Start_Recursion(node->pChildren[i]);
    }
}


void Octree::DivideOrAdd(Node* node, Triangle tri)
{

    for (int i = 0; i < 8; ++i)
    {
        if (Tri_check(tri, node->pChildren[i]->aabb))
        {
            // add this triangle to this children if all vert is in this AABB
            node->pChildren[i]->triangles.push_back(tri);
            return;
        }
    }

    // need to Divide triangle!
    std::stack<Triangle> triangles_stack;
    triangles_stack.push(tri);

    for (int i = 0; i < 3; i++) // x,y,z
    {
        std::vector<Triangle> axis_result_tri;

        while (!triangles_stack.empty())
        {
            Triangle triangle = triangles_stack.top();

            auto newTriangleInfo = IntersectRayPlane(node, i, triangle);
            bool did_divide = newTriangleInfo.first;

            if (did_divide == true)
            {
                auto newTriangles = newTriangleInfo.second;
                axis_result_tri.insert(axis_result_tri.end(), newTriangles.begin(), newTriangles.end());
            }
            else
            {
                axis_result_tri.push_back(triangle);
            }

            triangles_stack.pop();
        }

        for (auto newTriangle : axis_result_tri)
        {
            // Add result triangles to stack for next axis divide calc
            triangles_stack.push(newTriangle);
        }
    }

    // x,y,z axis divided triangles result are in stack.
    while (!triangles_stack.empty())
    {
        auto newTriangle = triangles_stack.top();

        for (int i = 0; i < 8; i++)
        {
            if (Tri_check(newTriangle, node->pChildren[i]->aabb))
            {
                node->pChildren[i]->triangles.push_back(newTriangle);
                break;
            }
        }
        triangles_stack.pop();
    }
}

//check if all vert of triangle is in AABB.
bool Octree::Tri_check(const Triangle& tri, AABB* aabb)
{
    for (int i = 0; i < 3; ++i) {
        const glm::vec3& v = tri.vertices[i];
        // AABB의 범위를 Epsilon만큼 살짝 늘려서 검사합니다.
        if (v.x < aabb->min.x - EPSILON || v.x > aabb->max.x + EPSILON ||
            v.y < aabb->min.y - EPSILON || v.y > aabb->max.y + EPSILON ||
            v.z < aabb->min.z - EPSILON || v.z > aabb->max.z + EPSILON)
        {
            return false;
        }
    }
    return true;
}

std::pair<bool, std::vector<Triangle>> Octree::IntersectRayPlane(Node* node, int axisIndex, Triangle tri)
{
    std::vector<Triangle> resultT;
    glm::vec3 normal;

    switch (axisIndex)
    {
    case 0: //X
        normal = glm::vec3(1, 0, 0);
        break;

    case 1: //Y
        normal = glm::vec3(0, 1, 0);
        break;

    case 2: //Z
        normal = glm::vec3(0, 0, 1);
        break;
    }
    std::vector<glm::vec3> front;
    std::vector<glm::vec3> back;
    std::vector<glm::vec3> inside;



    for (int i = 0; i < 3; ++i)
    {
        //if (node->aabb->center[axisIndex] < v.vertices[i][axisIndex])
        //    front.push_back(v.vertices[i]);
        //else if (node->aabb->center[axisIndex] > v.vertices[i][axisIndex])
        //    back.push_back(v.vertices[i]);
        //else
        //    inside.push_back(v.vertices[i]);


        float diff = tri.vertices[i][axisIndex] - node->aabb->center[axisIndex];

        if (diff > EPSILON)
            front.push_back(tri.vertices[i]);
        else if (diff < -EPSILON)
            back.push_back(tri.vertices[i]);
        else // abs(diff) <= EPSILON
            inside.push_back(tri.vertices[i]);

    }

    if (back.empty() || front.empty())
        return { false, resultT };

    if (!inside.empty())
    {
        glm::vec3 v1 = front[0];
        glm::vec3 v2 = back[0];
        glm::vec3 dir = glm::normalize(v2 - v1);
        float distance = 0.0f;

        bool intersect = glm::intersectRayPlane(v1, dir, node->aabb->center, normal, distance);

        if (!intersect)
            return { false, resultT };

        glm::vec3 insideV = inside[0];
        glm::vec3 i1 = v1 + distance * dir;

        Triangle t1{ v1, i1, insideV };
        Triangle t2{ i1, v2, insideV };

        resultT.insert(resultT.end(), { t1,t2 });
    }
    else
    {
        glm::vec3 v1;
        glm::vec3 v2;
        glm::vec3 v3;
        if (back.size() < front.size())
        {
            v1 = back[0];
            v2 = front[0];
            v3 = front[1];
        }
        else
        {
            v1 = front[0];
            v2 = back[0];
            v3 = back[1];
        }

        glm::vec3 dir = glm::normalize(v2 - v1);
        float distance;

        bool intersect = glm::intersectRayPlane(v1, dir, node->aabb->center, normal, distance);
        if (!intersect)
            return { false, resultT };
        glm::vec3 i1 = v1 + distance * dir;

        dir = glm::normalize(v3 - v1);
        intersect = glm::intersectRayPlane(v1, dir, node->aabb->center, normal, distance);
        if (!intersect)
            return { false, resultT };
        glm::vec3 i2 = v1 + distance * dir;

        Triangle t1{ v1, i2, i1 };
        Triangle t2{ i1, i2, v2 };
        Triangle t3{ i2, v3, v2 };
        resultT.insert(resultT.end(), { t1,t2,t3 });
    }

    return { true, resultT };
}

void Octree::Render_PreorderTraversal(GLSLShader& shader, Node* node)
{
    if (node == nullptr)
    {
        return;
    }

    // --- 1. 현재 노드의 AABB 그리기 ---
    // (AABB를 그릴 때마다 데이터를 업데이트하는 동적 VBO 방식 사용)
    //glBindVertexArray(TREE_VAO);
    //glBindBuffer(GL_ARRAY_BUFFER, TREE_VBO);

    if (show_AABB)
    {
        if (show_AABB_Level[node->level])
        {
            glBufferData(GL_ARRAY_BUFFER, node->aabb->lines.size() * sizeof(glm::vec3), &node->aabb->lines[0], GL_STATIC_DRAW);

            glUniform4fv(glGetUniformLocation(shader.GetHandle(), "color"), 1, glm::value_ptr(colors[node->level])); // Green for AABB
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(node->aabb->lines.size()));
        }
    }


    // --- 2. 로직 분기: 리프 노드인가, 내부 노드인가? ---

    // 이 노드가 리프 노드라면 (삼각형을 가지고 있다면), 삼각형을 그립니다.
    if (!node->triangles.empty()) // 혹은 isLeaf() 같은 플래그 사용
    {
        std::vector<glm::vec3> temp_vertices;
        temp_vertices.reserve(node->triangles.size() * 3); // 미리 메모리 할당
        for (const auto& tri : node->triangles)
        {
            temp_vertices.push_back(tri.vertices[0]);
            temp_vertices.push_back(tri.vertices[1]);
            temp_vertices.push_back(tri.vertices[2]);
        }

        // 삼각형용 VBO에 데이터 업데이트 (VAO/VBO를 공유한다면 같은 VBO 사용)
        // 여기서는 설명을 위해 TRIANGLE_VBO를 사용한다고 가정
        glBufferData(GL_ARRAY_BUFFER, temp_vertices.size() * sizeof(glm::vec3), temp_vertices.data(), GL_STATIC_DRAW);

        glUniform4fv(glGetUniformLocation(shader.GetHandle(), "color"), 1, glm::value_ptr(colors[node->level]));
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(temp_vertices.size()));
    }
    // 이 노드가 내부 노드라면 (자식이 있다면), 자식들을 재귀 호출합니다.
    else
    {
        for (Node* child : node->pChildren)
        {
            Render_PreorderTraversal(shader, child);
        }
    }

    // 마지막에 VAO 바인딩 해제
    //glBindVertexArray(0);
}

void Octree::Clean_PreorderTraversal(Node* node)
{
    if (node != nullptr)
    {
        for (auto n : node->pChildren)
        {
            Clean_PreorderTraversal(n);
        }
        SafeDelete(node->aabb);
        SafeDelete(node);
    }
}
