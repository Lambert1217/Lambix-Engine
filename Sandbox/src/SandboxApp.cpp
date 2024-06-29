#include <Lambix.h>
#include "Lambix/Core/EntryPoint.h"
#include "Sandbox2D.h"

class Sandbox : public Lambix::Application
{
public:
	Sandbox(const Lambix::WindowProps& props = Lambix::WindowProps())
		: Lambix::Application(props)
	{
		PushLayer(new Sandbox2D(props.Width, props.Height));
	}
	~Sandbox()
	{}
};

Lambix::Application* Lambix::CreateApplication()
{
	return new Sandbox({ "Lambix", 1920, 1080 });
}