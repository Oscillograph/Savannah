#include <savannah/platforms/opengl/opengl_context.h>

namespace Savannah
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
	: m_Window(window)
	{}
	
	void OpenGLContext::Init(){
		SAVANNAH_CORE_INFO("GLFW window created.");
		
		glfwMakeContextCurrent(m_Window);
		
		SAVANNAH_CORE_INFO("GLFW window context set.");
		
		// int version = gladLoadGL((GLADloadproc)glfwGetProcAddress);
		int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SAVANNAH_CORE_ASSERT(version, "Failed to initialize Glad");
		//BSE_CORE_INFO("Glad initialized: {0}.{1}", GLFW_VERSION_MAJOR(version), GLFW_VERSION_MINOR(version));
		SAVANNAH_CORE_INFO("Glad initialized: {0}", version);
		
		SAVANNAH_CORE_INFO("OpenGL Info.");
		SAVANNAH_CORE_INFO("Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		SAVANNAH_CORE_INFO("Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		SAVANNAH_CORE_INFO("Version: {0}", (const char*)glGetString(GL_VERSION));
	}
	
	void OpenGLContext::SwapBuffers(){
		glfwSwapBuffers(m_Window);
	}
	
	void OpenGLContext::CopyBuffers(){
		glReadBuffer(GL_BACK);
		glDrawBuffer(GL_FRONT);
		int w, h;
		glfwGetWindowSize(m_Window, &w, &h);
		glBlitFramebuffer(	0, 0, w, h,
		0, 0, w, h, 
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
}

