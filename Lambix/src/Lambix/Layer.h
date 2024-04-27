#pragma once

#include "Lambix/core.h"
#include "Lambix/Events/Event.h"

namespace Lambix
{

	class Layer
	{
	public:
		Layer(const std::string &name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}			  // 图层附加
		virtual void OnDetach() {}			  // 图层分离
		virtual void OnUpdate() {}			  // 图层更新
		virtual void OnImGuiRender() {}		  // 在 ImGui 渲染时调用
		virtual void OnEvent(Event &event) {} // 事件回调

		inline const std::string &GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}