//#pragma once
//#include "EngineCore.h"
//
//namespace Graphics
//{
//    class Engine_API Application
//    {
//    public:
//        Application();
//        virtual ~Application();
//
//        // 사용자가 이 함수들을 오버라이드하여 채워넣어야 합니다.
//        virtual void OnCreate() = 0;
//        virtual void OnUpdate(float deltaTime) = 0;
//        virtual void OnRender() = 0;
//        virtual void OnDestroy() = 0;
//    };
//
//    // Engine 외부에서 이 함수를 정의하여 Application 인스턴스를 생성하고 반환해야 합니다.
//    // Engine은 이 함수를 호출하여 사용자가 만든 Application을 가져옵니다.
//    Application* CreateApplication();
//};