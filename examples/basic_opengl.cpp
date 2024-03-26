#include "./basic_opengl/include/basic_opengl.h"

namespace Savannah 
{
	Basic::Basic()
	{
		SAVANNAH_CONSOLE_GREEN("Savannah Framework initialization succesful.");
		SetWindowTitle("Профориентатор 1.0");
		
		SAVANNAH_CONSOLE_LOG("Enter Idle mode");
		
		SAVANNAH_CONSOLE_LOG("Add a new Task: LoadData");
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
