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
        // GLFW, OpenGL 초기화 등...

        // 사용자가 정의한 Application을 생성합니다.
        m_Application.reset(CreateApplication());
    }

    Engine::~Engine()
    {
        m_Application->OnDestroy();
    }

    void Run()
    {
        m_Application->OnCreate();

        // --- 메인 루프 ---
        while (m_IsRunning)
        {
            // 1. 입력 처리 (Poll Events)
            // 2. 시간 계산 (Delta Time)
            float deltaTime = 0.016f; // 예시

            // 3. 업데이트 (사용자 로직)
            m_Application->OnUpdate(deltaTime);

            // 4. 렌더링 (화면 그리기)
            // glClearColor, glClear 등...
            m_Application->OnRender();

            // 5. 버퍼 스왑
            // glfwSwapBuffers(m_Window);
        }
    }
}
