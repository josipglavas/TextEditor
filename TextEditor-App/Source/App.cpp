#include "Core/Core.h"
#include <d3d9.h>

#if defined(_DEBUG) || defined(RELEASE)
int main() {
	return TextEditorCore::RunApp();
}
#endif

#ifdef DIST
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return TextEditorCore::RunApp();
}
#endif