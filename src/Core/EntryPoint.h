#pragma once
#include "Core/Application.h"

#ifdef TS_PLATFORM_WINDOWS

extern TS_ENGINE::Application* TS_ENGINE::CreateApplication(std::filesystem::path exeDir);

int main(int argc, char** argv)
{
	TS_ENGINE::Log::Init();
	auto app = TS_ENGINE::CreateApplication(std::filesystem::path(argv[0]).parent_path());
	app->Run();
	delete app;
}

#endif