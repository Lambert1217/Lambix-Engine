#include "Lambix.h"

class Sandbox : public Lambix::Application
{
};

Lambix::Application *Lambix::CreateApplication()
{
    return new Sandbox();
}