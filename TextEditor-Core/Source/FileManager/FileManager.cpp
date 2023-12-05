#include "FileManager.h"
#include <vector>
#include <sstream>
#include <codecvt>
#include <locale>


namespace TextEditorCore {
	void OpenFile(ImVector<char>* ptrStrToOverwrite) {
		OPENFILENAMEW ofn;
		wchar_t szFile[260] = { 0 }; // Initialize with zeros

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
		ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameW(&ofn) == TRUE) {
			// Check if the selected file has a .txt extension
			std::wstring selectedFile = ofn.lpstrFile;
			if (selectedFile.size() < 4 || selectedFile.substr(selectedFile.size() - 4) != L".txt") {
				std::wcerr << L"Selected file is not a .txt file" << std::endl;
				return; // Log an error and return
			}

			std::wifstream file(ofn.lpstrFile, std::ios::binary);

			if (file.is_open()) {
				// Read the file content into a wide string
				std::wstringstream wss;
				wss << file.rdbuf();
				std::wstring content = wss.str();

				// Convert the wide string to a multi-byte string
				std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
				std::string multiByteStr = converter.to_bytes(content);

				// Clear and then resize the ImVector<char>
				ptrStrToOverwrite->clear();
				ptrStrToOverwrite->resize(multiByteStr.size());

				// Copy the multi-byte string to the ImVector<char>
				std::copy(multiByteStr.begin(), multiByteStr.end(), ptrStrToOverwrite->begin());

				file.close();
				return; // File successfully opened and read
			} else {
				std::wcerr << L"Unable to open file" << std::endl;
				return; // Failed to open file
			}
		} else {
			std::wcout << L"User canceled the operation" << std::endl;
			return; // User canceled the file dialog
		}
	}


	//void OpenFile(ImVector<char>* ptrStrToOverwrite) {
	//	OPENFILENAMEW ofn;
	//	wchar_t szFile[260] = { 0 }; // Initialize with zeros

	//	ZeroMemory(&ofn, sizeof(ofn));
	//	ofn.lStructSize = sizeof(ofn);
	//	ofn.hwndOwner = NULL;
	//	ofn.lpstrFile = szFile;
	//	ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
	//	ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	//	ofn.nFilterIndex = 1;
	//	ofn.lpstrFileTitle = NULL;
	//	ofn.nMaxFileTitle = 0;
	//	ofn.lpstrInitialDir = NULL;
	//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//	if (GetOpenFileNameW(&ofn) == TRUE) {
	//		// Check if the selected file has a .txt extension
	//		std::wstring selectedFile = ofn.lpstrFile;
	//		if (selectedFile.size() < 4 || selectedFile.substr(selectedFile.size() - 4) != L".txt") {
	//			std::wcerr << L"Selected file is not a .txt file" << std::endl;
	//			return; // Log an error and return
	//		}

	//		std::wifstream file(ofn.lpstrFile, std::ios::binary);

	//		if (file.is_open()) {
	//			// Read the file content into a string
	//			std::wstring content((std::istreambuf_iterator<wchar_t>(file)), std::istreambuf_iterator<wchar_t>());

	//			// Convert the wide string to a multi-byte string
	//			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	//			std::string multiByteStr = converter.to_bytes(content);

	//			// Copy the multi-byte string to the ImVector<char>
	//			ptrStrToOverwrite->resize(multiByteStr.size());
	//			std::copy(multiByteStr.begin(), multiByteStr.end(), ptrStrToOverwrite->begin());

	//			file.close();
	//			return; // File successfully opened and read
	//		} else {
	//			std::wcerr << L"Unable to open file" << std::endl;
	//			return; // Failed to open file
	//		}
	//	} else {
	//		std::wcout << L"User canceled the operation" << std::endl;
	//		return; // User canceled the file dialog
	//	}
	//}

	void SaveFile(const ImVector<char>* textInTheEditor) {
		OPENFILENAMEW ofn;
		wchar_t szFile[MAX_PATH] = { 0 }; // Initialize with zeros

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
		ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

		if (GetSaveFileNameW(&ofn) == TRUE) {
			// Ensure the file path has the .txt extension
			std::wstring filePath = ofn.lpstrFile;
			if (filePath.length() < 4 || filePath.substr(filePath.length() - 4) != L".txt") {
				filePath += L".txt";
			}

			std::wofstream file(filePath);
			if (file.is_open()) {
				// Iterate only over the actual data and skip the null-terminator
				for (size_t i = 0; i < textInTheEditor->size(); ++i) {
					char currentChar = (*textInTheEditor)[i];

					// Skip null-terminator
					if (currentChar != '\0') {
						file << static_cast<wchar_t>(currentChar);
					}
				}

				file.close();
				std::wcout << L"File saved successfully: " << filePath << std::endl;
			} else {
				std::wcerr << L"Unable to save file" << std::endl;
			}

		} else {
			std::wcout << L"User canceled the operation" << std::endl;
		}
	}
}