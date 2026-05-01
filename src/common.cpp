#pragma once
#include <filesystem>
#include <libloaderapi.h>
#include <string>
#include <vector>

using namespace std;

filesystem::path getExecutiblePath();
filesystem::path getExecutiblePath() {
	wchar_t path[FILENAME_MAX] = { 0 };
	GetModuleFileNameW(nullptr, path, FILENAME_MAX);
	return filesystem::path(path);
}

vector<string> splitString(const string& text, const string& delimiter);
vector<string> splitString(const string& text, const string& delimiter) {
	vector<string> output {};

	size_t from = 0;
	size_t pos;

	while ((pos = text.find(delimiter, from)) != string::npos) {
		output.push_back(text.substr(from, pos - from));
		from = pos + delimiter.length();
	}

	output.push_back(text.substr(from));

	return output;
}