#include <include/savannah/savannah.h>

namespace ImPlot
{
	template <typename T>
	inline T RandomRange(T min, T max) {
		T scale = rand() / (T) RAND_MAX;
		return min + scale * ( max - min );
	}
}

namespace Savannah 
{
	class ImPlotTest : public App 
	{
	public:
		ImPlotTest();
		~ImPlotTest();
		
		void SetupResources() override;
		void Logic() override;
		void GUIContent() override;		
	private:
		float TEXT_BASE_WIDTH = 0.0f;
		float TEXT_BASE_HEIGHT = 0.0f;
		
		void ShowMainMenu();
	};
}
