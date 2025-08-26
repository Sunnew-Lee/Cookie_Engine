#pragma once
#include "EngineCore.h"


namespace Graphics
{
    class Application;

    class Engine_API Engine
    {
    public:
        Engine();
        ~Engine();

        void Run();

    private:
        Application* m_Application;
        bool m_IsRunning = true;
        // Window* m_Window;
        // ... ��Ÿ ����ý���

        static Engine* instance;
    };
};

