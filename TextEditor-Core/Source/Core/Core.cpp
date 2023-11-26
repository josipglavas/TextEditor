#include "Core.h"
#include "../ImGui/Gui.h"
#include <thread>
namespace TextEditorCore {
	int WINAPI RunApp() {
		CreateHWindow(L"Josip's text editor", L"Text Editor Class");
		CreateDevice();
		CreateImGui();

		while (appRunning) {

			BeginRender();
			Render();
			EndRender();

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		DestroyImGui();
		DestroyDevice();
		DestroyHWindow();

		return EXIT_SUCCESS;
	}
}