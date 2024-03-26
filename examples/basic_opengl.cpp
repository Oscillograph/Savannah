#include "./basic_opengl/include/basic_opengl.h"

namespace Savannah 
{
	Basic::Basic()
	{
		SAVANNAH_CONSOLE_GREEN("Savannah Framework initialization succesful.");
		SetWindowTitle("Basic 1.0");
	}
	
	Basic::~Basic()
	{
	}
	
	App* CreateApplication()
	{
		Basic* app = new Basic();
		app->SetFPS(SAVANNAH_FPS60);
		return app;
	}
}
