#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <vector>

struct DisplayInfo {
	DEVMODE dm;
	DISPLAY_DEVICE dd;
};
std::vector<DisplayInfo> DisplayVector;

BOOL CALLBACK EnumMonitorProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	static int monitor = 0;
	char szSaveDeviceName[33];  // 32 + 1 for the null-terminator 

	DISPLAY_DEVICE dd;
	dd.cb = sizeof(DISPLAY_DEVICE);
	EnumDisplayDevices(NULL, monitor, &dd, NULL);
	strcpy_s(szSaveDeviceName, 33, dd.DeviceName);
	EnumDisplayDevices(szSaveDeviceName, 0, &dd, NULL);
	// EnumDisplayDevices will provide the adapter name at the first call, and the real display name upon the second call

	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(szSaveDeviceName, ENUM_CURRENT_SETTINGS, &dm);
	DisplayVector.push_back({ dm, dd });
	monitor++;
	return TRUE;
}

int main(int argc, const char* argv[]) {
	EnumDisplayMonitors(NULL, NULL, EnumMonitorProc, 0);
	for (int i = 0; i < DisplayVector.size(); i++) {
		std::cout << "[" << i << "]: " << DisplayVector[i].dd.DeviceString << "(" << DisplayVector[i].dd.DeviceName << ")" << std::endl;
	}
	std::cout << "Select display (0-" << DisplayVector.size() - 1 << "): ";
	int selection;
	std::cin >> selection;
	std::cout << "Refresh Rate: " << DisplayVector[selection].dm.dmDisplayFrequency << "Hz" <<  std::endl;
	std::cout << "Width (Pixels): " << DisplayVector[selection].dm.dmPelsWidth << std::endl;
	std::cout << "Height (Pixels): " << DisplayVector[selection].dm.dmPelsHeight << std::endl;
	return EXIT_SUCCESS;
}
