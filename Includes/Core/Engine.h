#pragma once
#include "EngineCore.h"

#include <string>

struct GLFWwindow;
class Scene;
class Camera;
struct ImGuiContext;

namespace Graphics
{
    //class Application;

    class Engine_API Engine
    {
    public:
        Engine();
        ~Engine();

        void Run();

        // 레벨 추가 함수.
        void AddScene(Scene* new_scene);

    protected:
        ImGuiContext* Gui{ nullptr };

    private:
        void Engine_Init();
        bool GLFW_Init(std::string);

        void Update(double delta_time);
        void Render();
        void CleanUp();

        double Update_Time(double fps_calc_interval);

    private:
        static constexpr int WINDOW_WIDTH = 1080;
        static constexpr int WINDOW_HEIGHT = 720;
        GLFWwindow* ptr_window{ nullptr };
        Camera* camera{ nullptr };

        Scene* main_scene{ nullptr };

        inline static Engine* instance = nullptr;
    };
};