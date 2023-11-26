#include "FileManager.h"

namespace TextEditorCore {

#include <vector>

	ImVector<char> OpenFile() {
		ImVector<char> VectorToReturn;

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
				return VectorToReturn; // Log an error and return
			}

			std::wifstream file(ofn.lpstrFile, std::ios::binary);

			if (file.is_open()) {
				const std::streamsize bufferSize = 512; // Adjust the buffer size as needed
				std::vector<wchar_t> buffer(bufferSize);

				while (file.read(buffer.data(), bufferSize)) {
					for (wchar_t c : buffer) {
						VectorToReturn.push_back(static_cast<char>(c));
					}
				}

				// Handle the remaining characters
				for (wchar_t c : std::wstring_view(buffer.data(), file.gcount())) {
					VectorToReturn.push_back(static_cast<char>(c));
				}

				file.close();
				return VectorToReturn; // File successfully opened and read
			} else {
				std::wcerr << L"Unable to open file" << std::endl;
				return VectorToReturn; // Failed to open file
			}
		} else {
			std::wcout << L"User canceled the operation" << std::endl;
			return VectorToReturn; // User canceled the file dialog
		}
	}

	//ImVector<char> OpenFile() {
	//	ImVector<char> VectorToReturn;

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
	//			return VectorToReturn; // Log an error and return
	//		}

	//		std::wifstream file(ofn.lpstrFile);

	//		if (file.is_open()) {
	//			while (true) {
	//				std::wstring line;
	//				if (!std::getline(file, line))
	//					break; // Exit loop if no more lines

	//				for (wchar_t c : line) {
	//					VectorToReturn.push_back(static_cast<char>(c));
	//				}

	//				VectorToReturn.push_back('\n'); // Add newline character
	//			}

	//			file.close();
	//			return VectorToReturn; // File successfully opened and read
	//		} else {
	//			std::wcerr << L"Unable to open file" << std::endl;
	//			return VectorToReturn; // Failed to open file
	//		}
	//	} else {
	//		std::wcout << L"User canceled the operation" << std::endl;
	//		return VectorToReturn; // User canceled the file dialog
	//	}
	//}
	//ImVector<char> OpenFile() {
	//	ImVector<char> VectorToReturn;

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
	//		std::wifstream file(ofn.lpstrFile);

	//		if (file.is_open()) {
	//			while (true) {
	//				std::wstring line;
	//				if (!std::getline(file, line))
	//					break; // Exit loop if no more lines

	//				for (wchar_t c : line) {
	//					VectorToReturn.push_back(static_cast<char>(c));
	//				}

	//				VectorToReturn.push_back('\n'); // Add newline character
	//			}

	//			file.close();
	//			return VectorToReturn; // File successfully opened and read
	//		} else {
	//			std::wcerr << L"Unable to open file" << std::endl;
	//			return VectorToReturn; // Failed to open file
	//		}
	//	} else {
	//		std::wcout << L"User canceled the operation" << std::endl;
	//		return VectorToReturn; // User canceled the file dialog
	//	}
	//}

	void SaveFile(const ImVector<char>& textInTheEditor) {
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
				for (char c : textInTheEditor) {
					file << static_cast<wchar_t>(c);
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