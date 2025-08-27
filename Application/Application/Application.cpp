#include "Application.h"
#include "../Scene/Demo.h"
//#include "Scene/Scene.h"

Application::Application()
{
	//AddScene(new ApplicationScene());
	AddScene(new Demo);
}

Application::~Application()
{
}
