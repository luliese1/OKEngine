#pragma once

int SetCurrentDir()
{
	const UINT maxPath = 100;

	wchar_t workingPath[maxPath];
	GetCurrentDirectory(maxPath, workingPath);
	wchar_t exePathchar[maxPath];
	GetModuleFileName(NULL, exePathchar, maxPath);

	std::string::size_type pos = std::wstring(exePathchar).find_last_of(L"\\/");

	std::wstring exePath(exePathchar);
	std::wstring exeDirectoryPath = exePath.substr(0, pos);

	int ret = SetCurrentDirectory(exeDirectoryPath.c_str());

	return ret;
}