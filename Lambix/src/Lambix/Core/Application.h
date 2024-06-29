#pragma once

#include "Core.h"
#include "Lambix/Events/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"
#include "Lambix/ImGui/ImGuiLayer.h"

/*
    应用类 单例
*/

namespace Lambix
{
    class Application
    {
    public:
        Application(const WindowProps& props);
        virtual ~Application();

        void Run(); // 运行 程序循环

        void OnEvent(Event& e); // 事件处理 可以逐层分发事件

        void PushLayer(Layer* layer);     // 添加层
        void PushOverlay(Layer* overlay); // 添加覆盖层 例如 imgui

        inline Window& GetWindow() { return *m_Window; }                 // 获取窗口类实例
        inline static Application& GetInstance() { return *m_Instance; } // 获取应用类单例
    private:
        bool OnWindowClose(WindowCloseEvent& e); // 窗口关闭事件处理函数
        bool OnWindowResize(WindowResizeEvent& e); // 窗口关闭事件处理函数
    private:
        static Application* m_Instance; // 应用类单例

        Scope<Window> m_Window; // 智能指针 窗口类实例
        ImGuiLayer* m_ImGuiLayer;         // ImGuiLayer
        bool m_Running{ true };             // 程序循环执行
        LayerStack m_LayerStack;          // 层 栈
        float LastFrameTime{ 0.0f };
        bool m_Minsize = false;
    };

    Application* CreateApplication(); // 创建应用  由用户定义
}
