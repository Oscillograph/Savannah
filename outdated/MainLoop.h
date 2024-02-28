#ifndef SAVANNAH_MAINLOOP_H
#define SAVANNAH_MAINLOOP_H

#include <Savannah/Core.h>

extern Savannah::Application* Savannah::CreateApplication(); 

int main(int argc, char** args) {
	// SAVANNAH_PROFILE_SESSION_START("Startup", "./debug/startup.json");
	// Savannah::Debugger::Init();
	SAVANNAH_CORE_WARN("Log System Initialized!\n");
	SAVANNAH_INFO("Log System Initialized!\n");
	auto app = Savannah::CreateApplication();
	// SAVANNAH_PROFILE_SESSION_END();
	
	// SAVANNAH_PROFILE_SESSION_START("Runtime", "./debug/runtime.json");
	app->Run();
	// SAVANNAH_PROFILE_SESSION_END();
	
	// SAVANNAH_PROFILE_SESSION_START("Shutdown", "./debug/shutdown.json");
	delete app;
	// Savannah::Profiler::Flush();
	// SAVANNAH_PROFILE_SESSION_END();
	return 0;
}

#endif
