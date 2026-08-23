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

	result = WlanScan(
		wlanhandle,
		&interfaceGuid,
		nullptr,
		nullptr,
		nullptr);

	if (result != ERROR_SUCCESS) {
		std::cout << "Error: Failed to start WLAN scan. Code: "
			<< result << std::endl;
		return 1;
	}

	std::cout << "WLAN scan started successfully!" << std::endl;

	DWORD dwflags = 0;
	PWLAN_AVAILABLE_NETWORK_LIST pNetworkList = nullptr;

	result = WlanGetAvailableNetworkList(
		wlanhandle,
		&interfaceGuid,
		dwflags,
		nullptr,
		&pNetworkList);

	if (result != ERROR_SUCCESS) {
		std::cout << "Error: Not found any available networks. Code: "
			<< result << std::endl;
		return 1;
	}

	std::cout << "Number of networks found: "
		<< pNetworkList->dwNumberOfItems
		<< std::endl;
	std::cout << "Available WLAN networks retrieved successfully!\n" << std::endl;

	PWLAN_BSS_LIST pBssList = nullptr;

	result = WlanGetNetworkBssList(
		wlanhandle,
		&interfaceGuid,
		nullptr,
		dot11_BSS_type_any, 
		FALSE,
		nullptr,
		&pBssList
	);

	if (result == ERROR_SUCCESS && pBssList != nullptr) {


		for (DWORD i = 0; i < pBssList->dwNumberOfItems; i++) {
			WLAN_BSS_ENTRY bssEntry = pBssList->wlanBssEntries[i];

			std::cout << "-----------------------------------" << std::endl;
			std::cout << "SSID: ";


			if (bssEntry.dot11Ssid.uSSIDLength > 0) {
				for (DWORD j = 0; j < bssEntry.dot11Ssid.uSSIDLength; j++) {
					std::cout << bssEntry.dot11Ssid.ucSSID[j];
				}
				std::cout << std::endl;
			}
			else {
				std::cout << "<Hidden network>" << std::endl;
			}


			std::cout << "Signal quality: " << bssEntry.lRssi << " dBm" << std::endl;
		}

		std::cout << "-----------------------------------" << std::endl;

		WlanFreeMemory(pBssList);
		pBssList = nullptr;
	}

	if (pInterfaceList != nullptr) {
		WlanFreeMemory(pInterfaceList);
		pInterfaceList = nullptr;
	}

	if (pNetworkList != nullptr) {
		WlanFreeMemory(pNetworkList);
		pNetworkList = nullptr;
	}

	if (wlanhandle != nullptr) {
		WlanCloseHandle(wlanhandle, nullptr);
		wlanhandle = nullptr;
	}

	return 0;
}