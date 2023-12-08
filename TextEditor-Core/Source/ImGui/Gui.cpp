#include "Gui.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <string>
#include <iostream>
#include "../FileManager/FileManager.h"
#include <dwmapi.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

struct Funcs {
	static int MyResizeCallback(ImGuiInputTextCallbackData* data) {
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
			ImVector<char>* my_str = (ImVector<char>*)data->UserData;
			IM_ASSERT(my_str->begin() == data->Buf);

			// Resize the vector
			my_str->resize(data->BufSize);

			// Null-terminate the buffer
			if (!my_str->empty() && (*my_str)[my_str->size() - 1] != '\0') {
				my_str->push_back('\0');
			}

			// Set the buffer pointer
			data->Buf = my_str->begin();

			// Check if the text is empty and reset vector size
			if (data->BufTextLen == 0) {
				my_str->clear();
			}
		}
		return 0;
	}

	static bool MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0) {
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);

		// Use InputTextMultiline with the ImGuiInputTextFlags_CallbackResize flag
		bool valueChanged = ImGui::InputTextMultiline(label, my_str->begin(), my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, MyResizeCallback, (void*)my_str);

		return valueChanged;
	}
};

namespace TextEditorCore {

	ImVector<char> my_str;

	ImFont* defaultFont = NULL;
	LRESULT WINAPI WindowProcess(
		HWND window,
		UINT message,
		WPARAM wideParameter,
		LPARAM longParameter
	) {
		if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
			return true;

		BOOL USE_DARK_MODE = true;
		BOOL SET_IMMERSIVE_DARK_MODE_SUCCESS = SUCCEEDED(DwmSetWindowAttribute(
			window, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
			&USE_DARK_MODE, sizeof(USE_DARK_MODE)));

		switch (message) {
		case WM_SIZE:
		{
			if (device && wideParameter != SIZE_MINIMIZED) {
				presentParameters.BackBufferWidth = LOWORD(longParameter);
				presentParameters.BackBufferHeight = HIWORD(longParameter);
				ResetDevice();
			}
		}return 0;

		case WM_SYSCOMMAND:
		{
			if ((wideParameter & 0xfff0) == SC_KEYMENU)
				return 0;
		}break;
		case WM_DESTROY:
		{
			appRunning = false;
			PostQuitMessage(0);
		}return 0;

		case WM_LBUTTONDOWN:
		{
			position = MAKEPOINTS(longParameter);
		}return 0;

		case WM_MOUSEMOVE:
		{
			if (wideParameter == MK_LBUTTON) {
				const auto points = MAKEPOINTS(longParameter);
				auto rect = ::RECT{};

				GetWindowRect(window, &rect);

				rect.left += points.x - position.x;
				rect.top += points.y - position.y;

				if (position.x >= 0 && position.x <= WIDTH && position.y >= 0 && position.y <= 19)
					SetWindowPos(window, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);

			}
		}return 0;

		}
		return DefWindowProcW(window, message, wideParameter, longParameter);

	}

	void CreateHWindow(LPCWSTR windowName, LPCWSTR className) noexcept {

		windowClass.cbSize = sizeof(WNDCLASSEXW);
		windowClass.style = CS_CLASSDC;
		windowClass.lpfnWndProc = WindowProcess;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandleA(0);
		windowClass.hIcon = 0;
		windowClass.hCursor = 0;
		windowClass.hbrBackground = 0;
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = className;
		windowClass.hIconSm = 0;

		RegisterClassExW(&windowClass);
		window = CreateWindowW(className, windowName, WS_OVERLAPPEDWINDOW, 0, 0, 1000, 700, nullptr, nullptr, windowClass.hInstance, nullptr);

		ShowWindow(window, SW_SHOWDEFAULT);
		UpdateWindow(window);
	}
	void DestroyHWindow() noexcept {
		DestroyWindow(window);
		UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);
	}

	bool CreateDevice() noexcept {
		d3d = Direct3DCreate9(D3D_SDK_VERSION);

		if (!d3d)
			return false;

		ZeroMemory(&presentParameters, sizeof(presentParameters));

		presentParameters.Windowed = TRUE;
		presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
		presentParameters.EnableAutoDepthStencil = TRUE;
		presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
		presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

		if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &device) < 0)
			return false;

		return true;
	}
	void ResetDevice() noexcept {
		ImGui_ImplDX9_InvalidateDeviceObjects();

		const auto result = device->Reset(&presentParameters);

		if (result == D3DERR_INVALIDCALL)
			IM_ASSERT(0);

		ImGui_ImplDX9_CreateDeviceObjects();
	}
	void DestroyDevice() noexcept {
		if (device) {
			device->Release();
			device = nullptr;
		}
		if (d3d) {
			d3d->Release();
			d3d = nullptr;
		}
	}

	void CreateImGui() noexcept {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ::ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

		io.IniFilename = NULL;

		ImGui::StyleColorsClassic();
		// Set the menu bar height
		ImGuiStyle& style = ImGui::GetStyle();
		style.FramePadding.y = 8.0f;  // Adjust the value according to your preference


		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(device);

		//LOAD CUSTOM FONTS HERE
		io.Fonts->AddFontDefault();
		defaultFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 18.0f);
		IM_ASSERT(defaultFont != NULL);

	}
	void DestroyImGui() noexcept {
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void BeginRender() noexcept {
		MSG message;
		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	void EndRender() noexcept {
		ImGui::EndFrame();

		device->SetRenderState(D3DRS_ZENABLE, FALSE);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

		if (device->BeginScene() >= 0) {
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			device->EndScene();
		}

		const auto result = device->Present(0, 0, 0, 0);

		if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			ResetDevice();
	}
	void Render() noexcept {
		ImGuiIO& io = ImGui::GetIO();

		//set window properties
		static bool no_titlebar = true;
		static bool auto_resize = true;
		static bool no_resize = true;
		static bool no_move = true;
		static bool no_scrollbar = false;
		static bool no_collapse = true;
		static bool menu = true;
		static bool no_settings = true;
		static float bg_alpha = -0.01f;

		ImGuiWindowFlags  window_flags = 0;
		if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
		if (auto_resize)  window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
		if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
		if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
		if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
		if (menu)         window_flags |= ImGuiWindowFlags_MenuBar;
		if (no_settings)  window_flags |= ImGuiWindowFlags_NoSavedSettings;

#ifdef IMGUI_HAS_VIEWPORT
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
#else 
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::Begin("Text Editor", &appRunning, window_flags);
		ImGui::PushFont(defaultFont);

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					TextEditorCore::OpenFile(&my_str);
				}

				if (ImGui::MenuItem("Save as ", "Ctrl+S")) {
					SaveFile(&my_str);
				}
				if (ImGui::MenuItem("Close", "Ctrl+Q")) {
					appRunning = false;
					PostQuitMessage(0);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		//KEYBOARD SHORTCUTS
		if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_O, false)) {
			TextEditorCore::OpenFile(&my_str);
			ImGui::PopFont();
			ImGui::End();
			ImGui::PopStyleVar(1);
			ImGui::Render();
			return;
		}
		if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
			SaveFile(&my_str);
		}
		if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Q, false)) {
			appRunning = false;
			PostQuitMessage(0);
		}

		if (my_str.empty())
			my_str.push_back(0);

		Funcs::MyInputTextMultiline("##MyStr", &my_str, ImVec2(-FLT_MIN, viewport->WorkSize.y * 0.93f));

		if (!my_str.empty() && my_str.back() != '\0') {
			my_str.push_back('\0');
		}

		ImGui::PopFont();
		ImGui::End();

		ImGui::PopStyleVar(1);
	}

}

