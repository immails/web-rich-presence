#include <cstddef>
#include <cstring>
#include <iostream>
#include <libloaderapi.h>
#include <minwindef.h>
#include <string>
#include <windows.h>
#include <winreg.h>
#include "./common.cpp"

static bool registerProtocol(bool launchedManually);
static bool registerProtocol(bool launchedManually) {
	HKEY hKey;
	
	if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Classes\\webrichpresence", 0, NULL, 0,
		KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
		std::cout << "Error creating key\n";
		if (launchedManually) MessageBox(NULL, "Failed to create registry key.", "WebRichPresence", MB_OK);
		return false;
	}

	const char* desc = "URL:WebRichPresence Protocol";
	RegSetValueExA(hKey, NULL, 0, REG_SZ, (BYTE*)desc, strlen(desc) + 1);

	const char* empty = "";
	RegSetValueExA(hKey, "URL Protocol", 0, REG_SZ, (BYTE*)empty, 1);

	RegCloseKey(hKey);

	if (RegCreateKeyExA(HKEY_CURRENT_USER,
		"Software\\Classes\\webrichpresence\\shell\\open\\command", 0, NULL, 0,
		KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
		std::cout << "Error creating command key\n";
		if (launchedManually) MessageBox(NULL, "Failed to create command registry key.", "WebRichPresence", MB_OK | MB_ICONERROR);
		return false;
	}

	std::string programPath = getExecutiblePath().string();
	std::string command = ("\"" + programPath + "\"" + " \"%1\"");
	RegSetValueExA(
		hKey, 
		NULL, 
		0, 
		REG_SZ, 
		(BYTE*) command.c_str(), 
		strlen(command.c_str()) + 1
	);

	RegCloseKey(hKey);

	if (launchedManually) MessageBox(NULL, "webrichpresence:// has been successfully registered. WebRichPresence will be launched automatically from now on!", "WebRichPresence", MB_OK);

	return true;
}