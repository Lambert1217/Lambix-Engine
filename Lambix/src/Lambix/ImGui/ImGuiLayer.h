#pragma once

#include "Lambix/Layer.h"

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
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time{0};
	};

}