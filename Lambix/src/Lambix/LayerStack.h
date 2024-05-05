#pragma once

#include "Lambix/Core.h"
#include "Layer.h"

#include "vector"

namespace Lambix
{

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer *layer);	  // 普通层入栈  从头部插入
		void PushOverlay(Layer *overlay); // 覆盖层入栈  从尾部插入
		void PopLayer(Layer *layer);
		void PopOverlay(Layer *overlay);

		// 方便外部遍历使用
		std::vector<Layer *>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer *>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer *> m_Layers;
		unsigned int m_LayerInsertIndex{0}; // 指向普通层最后一个
	};

}