#include "Engine.h"
#include "Application.h"

#include <iostream>

namespace Graphics 
{
    Engine* Engine::instance = nullptr;

    extern Application* CreateApplication();

    Engine::Engine()
    {
        std::cout << "Engine Core Initialized" << std::endl;
        // GLFW, OpenGL �ʱ�ȭ ��...

        // ����ڰ� ������ Application�� �����մϴ�.
        m_Application.reset(CreateApplication());
    }

    Engine::~Engine()
    {
        m_Application->OnDestroy();
    }

    void Run()
    {
        m_Application->OnCreate();

        // --- ���� ���� ---
        while (m_IsRunning)
        {
            // 1. �Է� ó�� (Poll Events)
            // 2. �ð� ��� (Delta Time)
            float deltaTime = 0.016f; // ����

            // 3. ������Ʈ (����� ����)
            m_Application->OnUpdate(deltaTime);

            // 4. ������ (ȭ�� �׸���)
            // glClearColor, glClear ��...
            m_Application->OnRender();

            // 5. ���� ����
            // glfwSwapBuffers(m_Window);
        }
    }
}
