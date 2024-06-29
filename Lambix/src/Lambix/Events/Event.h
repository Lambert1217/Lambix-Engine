#pragma once

#include "lbpch.h"
#include "Lambix/Core/Core.h"

namespace Lambix
{

	/**
		当前的事件系统仍是立即执行，即事件触发后立即处理
		未来可能会实现事件缓冲
	*/

	// 枚举事件类型
	enum class EventType
	{
		None = 0,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,
		WindowMoved,
		AppTick,
		AppUpdate,
		AppRender,
		KeyPressed,
		KeyReleased,
		KeyTyped,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	// 枚举事件大类  用于过滤
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	// 定义一些宏 用于批量生成派生类中的重复性函数
#define EVENT_CLASS_TYPE(type)                                                  \
	static EventType GetStaticType() { return EventType::type; }                \
	virtual EventType GetEventType() const override { return GetStaticType(); } \
	virtual const char *GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
	virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
		// 友元类 事件调度器
		friend class EventDispatcher;

	public:
		// 返回事件类型
		virtual EventType GetEventType() const = 0;

		// 返回名称
		virtual const char *GetName() const = 0;

		// 返回事件大类的筛选条件  （EventCategory 通过 或运算 得到）
		virtual int GetCategoryFlags() const = 0;

		// 名称转成字符串  （派生类可以重写用于输出一些额外信息）
		virtual std::string ToString() const { return GetName(); }

		// 判断 EventCategory 是否在筛选条件中
		inline bool IsInCategory(EventCategory category)
		{
			// 通过 与运算 返回结果
			return GetCategoryFlags() & category;
		}

		// 返回Handled
		inline bool GetHandled() { return Handled; }

	protected:
		// 事件会逐层发布，此变量用于记录是否已经被处理
		bool Handled = false;
	};

	// 重载 << 操作符， 输出事件信息
	inline std::ostream &operator<<(std::ostream &os, const Event &e)
	{
		return os << e.ToString();
	}

	// 事件调度器类
	class EventDispatcher
	{
		template <typename T>
		using EventFun = std::function<bool(T &)>;

	public:
		EventDispatcher(Event &event) : m_Event(event) {}

		template <typename T>
		bool Dispatch(EventFun<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T *)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event &m_Event;
	};
}