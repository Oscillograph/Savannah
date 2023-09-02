#ifndef SAVANNAH_OPENGLCONTEXT_H
#define SAVANNAH_OPENGLCONTEXT_H

#include <Savannah/Core.h>
#include <Savannah/Core/GraphicsContext.h>
#include <Savannah/Vendor/Glad/include/glad/glad.h>
#include <Savannah/Vendor/glfw/glfw3.h>

// struct GLFWwindow;

namespace Savannah {
	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* window);
		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void CopyBuffers() override;
	protected:
		GLFWwindow* m_Window;
	};
}

#endif
