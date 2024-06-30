#include <Lambix.h>
#include "Lambix/Core/EntryPoint.h"
#include "GameLayer.h"

class Sandbox : public Lambix::Application
{
public:
	Sandbox(const Lambix::WindowProps& props = Lambix::WindowProps())
		: Lambix::Application(props)
	{
		PushLayer(new GameLayer(props.Width, props.Height));
	}
	~Sandbox()
	{}
};

Lambix::Application* Lambix::CreateApplication()
{
	return new Sandbox({ "Lambix", 1080, 1080 });
}