#pragma once
#include "Core/Core.h"
#include <d3d9.h>
namespace TextEditorCore {
	constexpr int WIDTH = 1280;
	constexpr int HEIGHT = 720;

	inline bool appRunning = true;

	inline HWND window = nullptr;
	inline WNDCLASSEXW windowClass = { };

	inline POINTS position = { };

	inline PDIRECT3D9 d3d = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline D3DPRESENT_PARAMETERS presentParameters = { };

	void CreateHWindow(LPCWSTR windowName, LPCWSTR className) noexcept;
	void DestroyHWindow() noexcept;

	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestroyDevice() noexcept;

	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;

}