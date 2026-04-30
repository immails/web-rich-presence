#include <filesystem>
#include <libloaderapi.h>

std::filesystem::path getExecutiblePath();
std::filesystem::path getExecutiblePath() {
	wchar_t path[FILENAME_MAX] = { 0 };
	GetModuleFileNameW(nullptr, path, FILENAME_MAX);
	return std::filesystem::path(path);
}