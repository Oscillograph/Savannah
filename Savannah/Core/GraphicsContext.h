#ifndef SAVANNAH_GRAPHICSCONTEXT_H
#define SAVANNAH_GRAPHICSCONTEXT_H

#include <Savannah/Core.h>

namespace Savannah {
	class GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void CopyBuffers() = 0;
	};	
}

#endif
