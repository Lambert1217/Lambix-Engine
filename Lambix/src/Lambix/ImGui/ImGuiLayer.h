#pragma once

#include "Lambix/Core/Layer.h"

#include "Lambix/Events/ApplicationEvent.h"
#include "Lambix/Events/KeyEvent.h"
#include "Lambix/Events/MouseEvent.h"

namespace Lambix
{

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	};

}