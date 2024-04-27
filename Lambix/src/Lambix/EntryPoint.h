#pragma once

// 判断平台为 Windows
#ifdef LB_PLATFORM_WINDOWS

// 创建窗口函数声明  声明该函数是在别的函数中声明的
extern Lambix::Application *Lambix::CreateApplication();

// 程序的入口
int main(int argc, char **argv)
{
	// 初始化日志
	Lambix::Log::init();

	LB_CLIENT_INFO("Welcome to Lambix Engine!");

	// 窗口创建
	auto app = Lambix::CreateApplication();
	// 窗口运行
	app->Run();

	return 0;
}

#endif // LB_PLATFORM_WINDOWS