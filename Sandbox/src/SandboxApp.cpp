#include <Lambix.h>
#include "Lambix/Core/EntryPoint.h"
#include "Sandbox2D.h"

class Sandbox : public Lambix::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}
	~Sandbox()
	{}
};

Lambix::Application* Lambix::CreateApplication()
{
	return new Sandbox();
}