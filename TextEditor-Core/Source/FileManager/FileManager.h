#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <commdlg.h>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include "imgui.h"

namespace TextEditorCore {

	ImVector<char> OpenFile();
	void SaveFile(const ImVector<char>& textInTheEditor);
}