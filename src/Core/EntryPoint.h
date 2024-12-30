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
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Initialize logger
	TS_ENGINE::Log::Init();
	
	// Create application
	TS_ENGINE::Application* application = TS_ENGINE::CreateApplication(std::filesystem::path(argv[0]).parent_path());
	
	// Run application
	application->Run();

	// Delete application
	delete application;
	application = nullptr;
	
	TS_CORE_INFO("Deleted application!");

	//_CrtDumpMemoryLeaks();
}

#endif