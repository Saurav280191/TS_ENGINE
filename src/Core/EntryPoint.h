#pragma once
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#endif

#include "Core/Application.h"
#include <crtdbg.h>

#ifdef TS_PLATFORM_WINDOWS

extern TS_ENGINE::Application* TS_ENGINE::CreateApplication(std::filesystem::path exeDir);

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	TS_ENGINE::Log::Init();
	auto app = TS_ENGINE::CreateApplication(std::filesystem::path(argv[0]).parent_path());
	app->Run();
	delete app;
	
	_CrtDumpMemoryLeaks();
}

#endif