#include <savannah/core.h>
#include <savannah/main-app.h>

extern Savannah::App* Savannah::CreateApplication();

int main(int, char**)
{
	Savannah::App* application = Savannah::CreateApplication();
	
	application->Init();
	
	float time_previous = (float)glfwGetTime();
	
	while ((!glfwWindowShouldClose(application->GetWindow())) && !application->doExit){
		float time_current = (float)glfwGetTime();
		
		glfwWaitEventsTimeout(application->GetFPS());
		
		application->ProcessInput();
		// App::Logic();
		
		if ((time_current - time_previous) >= application->GetFPS()){
			time_previous = time_current;
			
			application->Render();
			// App::Sound();
		}
	}
	
	application->ShutDown();
	delete application;
	return 0;
}
