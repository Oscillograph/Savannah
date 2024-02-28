//#include <Window.h>
#include <Savannah/Platforms/Windows/WindowsWindow.h>

#include <Savannah/Systems/Events/AppEvent.h>
#include <Savannah/Systems/Events/KeyEvent.h>
#include <Savannah/Systems/Events/MouseEvent.h>

namespace Savannah {
	// a global variable in BSE namespace
	bool s_GLFWInitialized = false;
	
	void GLFWErrorCallback(int error, const char* text){
		SAVANNAH_CORE_ERROR("GLFW Error {0}: {1}", error, text);
	}
	
	Window* Window::Create(WindowProperties& properties){
		SAVANNAH_CORE_TRACE("Calling a Windows platform window creation function");
		return new WindowsWindow(properties);
	}
	
	WindowsWindow::WindowsWindow(const WindowProperties& properties){
		Init(properties);
	}
	
	WindowsWindow::~WindowsWindow(){
		SAVANNAH_CORE_TRACE("Window shutdown.");
		Shutdown();
	}
	
	void WindowsWindow::Init(const WindowProperties& properties){
		m_Data.Title = properties.Title;
		m_Data.Width = properties.Width;
		m_Data.Height = properties.Height;
		m_Data.VSync = true;
		m_Data.EventCallback = Callback_dummy;
		
		SAVANNAH_CORE_INFO("Window initialization start {0}, {1}, {2}", properties.Title, properties.Width, properties.Height);
		
		if (!s_GLFWInitialized){
			int success = glfwInit();
			if (!success){
				SAVANNAH_CORE_ERROR("Could not initialize GLFW!");
				exit(0);
			}
			SAVANNAH_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}
		
		SAVANNAH_CORE_INFO("GLFW Initialized {0}", s_GLFWInitialized);
		
		m_Window = glfwCreateWindow((int)properties.Width, 
			(int)properties.Height, m_Data.Title.c_str(), 
			nullptr, 
			nullptr);
		
		m_GraphicsContext = new OpenGLContext(m_Window);
		
		m_GraphicsContext->Init();
		
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SAVANNAH_CORE_INFO("GLFW user pointer attached to the window.");
		
		SetVSync(true);
		SAVANNAH_CORE_INFO("VSync set");
		
		// set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height){
			WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});
		
		/*
		  // this one is required for my AMD Mobility Radeon 6650M to change framebuffer size properly
		  glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height){
		  WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
		  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
		  // glViewport(0, 0, width, height);
		  SAVANNAH_CORE_INFO("Framebuffer resized to: {0}, {1}", width, height);
		  });
		 */
		
		
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window){
			WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});
		
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
			WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
			
			switch(action){
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1); // TODO: counter instead of 1 repeats
					data.EventCallback(event); // TODO: DRY!!!111111111111
					break;
				}
			}
		});
		
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key){
			WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
			
			KeyTypedEvent event(key);
			data.EventCallback(event);
		});
		
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods){
			WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
			switch(action){
				case GLFW_PRESS:{
					MouseButtonPressed event(button, 0); // TODO: decide for repeats
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:{
					MouseButtonReleased event(button);
					data.EventCallback(event);
					break;
				}
			}
		});
		
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset){
			WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});
		
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos){
			WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}
	
	void WindowsWindow::Shutdown(){
		glfwDestroyWindow(m_Window);
		//gladLoaderUnloadGL();
	}
	
	void WindowsWindow::OnUpdate(int code) const {
		glfwPollEvents();
		if ((m_Data.Width != 0) && (m_Data.Height != 0)){
			if (code == 0){
				m_GraphicsContext->CopyBuffers();
			} else {
				m_GraphicsContext->SwapBuffers();
			}
		}
	}
	
	void WindowsWindow::SetVSync(bool enabled){
		if (enabled){
			glfwSwapInterval(1);
		} else {
			glfwSwapInterval(0);
		}
		
		m_Data.VSync = enabled;
	}
	
	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}
}
