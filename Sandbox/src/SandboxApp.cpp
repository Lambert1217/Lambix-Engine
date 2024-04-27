#include <Lambix.h>
#include "imgui.h"

class ExampleLayer : public Lambix::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		if (Lambix::Input::IsKeyPressed(LB_KEY_SPACE))
		{
			LB_CLIENT_TRACE("KEY_SPACE is pressed!");
		}
		if (Lambix::Input::IsMouseButtonPressed(LB_MOUSE_BUTTON_1))
		{
			LB_CLIENT_TRACE("MOUSE_BUTTON_1 is pressed!");
		}
	}

	void OnEvent(Lambix::Event &event) override
	{
	}

	void OnImGuiRender() override
	{
		ImGui::Begin(GetName().c_str());
		ImGui::Text("Welcome to Lambix!");
		ImGui::End();
	}
};

class Sandbox : public Lambix::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{
	}
};

Lambix::Application *Lambix::CreateApplication()
{
	return new Sandbox();
}