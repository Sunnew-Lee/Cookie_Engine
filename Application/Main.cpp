#include "Application/Application.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(235);
	
	Application* app = new Application();
	app->Run();
	SafeDelete(app);
}