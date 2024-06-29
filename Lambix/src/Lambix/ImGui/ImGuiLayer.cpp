#include "lbpch.h"
#include "ImGuiLayer.h"

#include "Lambix/Core/Application.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Lambix
{

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	// 在图层附加时调用
	void ImGuiLayer::OnAttach()
	{
		// 设置 ImGui 上下文
		IMGUI_CHECKVERSION();	// 检查 ImGui 版本
		ImGui::CreateContext(); // 创建 ImGui 上下文
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 启用键盘控制
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // 启用游戏手柄控制
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// 启用停靠功能
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // 启用多视口 / 平台窗口
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// 设置 Dear ImGui 样式
		ImGui::StyleColorsClassic(); // 设置深色风格
		// ImGui::StyleColorsClassic();

		// 当启用多视口时，我们调整 WindowRounding/WindowBg，以便平台窗口看起来与常规窗口相同。
		ImGuiStyle &style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// 获取 GLFW 窗口指针
		Application &app = Application::GetInstance();
		GLFWwindow *window = static_cast<GLFWwindow *>(app.GetWindow().GetNativeWindow());

		// 设置平台/渲染器绑定
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460 core");
	}

	// 在图层分离时调用
	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown(); // 关闭 ImGui 对 OpenGL 3 的支持
		ImGui_ImplGlfw_Shutdown();	  // 关闭 ImGui 对 GLFW 的支持
		ImGui::DestroyContext();	  // 销毁 ImGui 上下文
	}

	// 开始 ImGui 渲染周期
	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame(); // 开始一个新的 ImGui 帧，为 OpenGL 准备新的渲染帧
		ImGui_ImplGlfw_NewFrame();	  // 在 GLFW 中开始一个新的 ImGui 帧
		ImGui::NewFrame();			  // 告诉 ImGui 开始一个新的帧
	}

	// 结束 ImGui 渲染周期
	void ImGuiLayer::End()
	{
		ImGuiIO &io = ImGui::GetIO();																	// 获取 ImGui 的输入输出对象
		Application &app = Application::GetInstance();													// 获取应用程序实例
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight()); // 设置 ImGui 的显示大小为应用程序窗口的大小

		// Rendering
		ImGui::Render();										// 渲染 ImGui 的绘制数据
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // 在 OpenGL 中渲染 ImGui 的绘制数据

		// 如果启用了多视口
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow *backup_current_context = glfwGetCurrentContext(); // 备份当前的 OpenGL 上下文
			ImGui::UpdatePlatformWindows();								  // 更新 ImGui 的平台窗口
			ImGui::RenderPlatformWindowsDefault();						  // 在默认平台上渲染 ImGui 窗口
			glfwMakeContextCurrent(backup_current_context);				  // 将原来的 OpenGL 上下文恢复为当前上下文
		}
	}
}