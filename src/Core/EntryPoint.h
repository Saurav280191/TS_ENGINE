#pragma once
#include "Core/Application.h"

#ifdef TS_PLATFORM_WINDOWS

extern TS_ENGINE::Application* TS_ENGINE::CreateApplication();

int main(int argc, char** argv)
{
	TS_ENGINE::Log::Init();

	auto app = TS_ENGINE::CreateApplication();
	app->Run();
	delete app;
}

#endif