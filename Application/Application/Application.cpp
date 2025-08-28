#include "Application.h"
#include "../Scene/Demo.h"

#include <imgui.h>

Application::Application()
{
	// need to use Engine initalized ImGui
	ImGui::SetCurrentContext(Graphics::Engine::Gui);

	AddScene(new Demo);
}

Application::~Application()
{
}
