#include "WifiAnalyzer.h"
#include <windows.h>
#include <wlanapi.h>
#include <iostream>

using namespace std;

int main()
{
	HANDLE wlanhandle = nullptr;
	DWORD negotiatedVersion = 0;

	DWORD result = WlanOpenHandle(2, nullptr, &negotiatedVersion, &wlanhandle);

	if (result != ERROR_SUCCESS) {
		std::cout << "Error: " << result << std::endl;
		return 1;
	}

	std::cout << "Wlan API connected successfully!" << std::endl;

	PVOID pReserved = nullptr;
	PWLAN_INTERFACE_INFO_LIST pInterfaceList = nullptr;

	result = WlanEnumInterfaces(
		wlanhandle,
		pReserved,
		&pInterfaceList
	);

	if (result != ERROR_SUCCESS) {
		std::cout << "Error: Failed to enumerate WLAN interfaces. Code: " << result << std::endl;
		WlanCloseHandle(wlanhandle, nullptr);
		return 1;
	}

	std::cout << "WLAN interfaces enumerated successfully!" << std::endl;

	std::cout << "Number of WLAN interfaces found: " << pInterfaceList->dwNumberOfItems << std::endl;

	std::wcout << "WLAN interface: " << pInterfaceList->InterfaceInfo[0].strInterfaceDescription << std::endl;

	GUID interfaceGuid = pInterfaceList->InterfaceInfo[0].InterfaceGuid;

	result = WlanScan(wlanhandle, &interfaceGuid, nullptr, nullptr, nullptr);




	if (pInterfaceList != nullptr) {
		WlanFreeMemory(pInterfaceList);
		pInterfaceList = nullptr;
	}

	if (wlanhandle != nullptr) {
		WlanCloseHandle(wlanhandle, nullptr);
		wlanhandle = nullptr;
	}

	return 0;
}