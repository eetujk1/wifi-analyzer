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

	result = WlanScan(wlanhandle, 
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

	result = WlanGetAvailableNetworkList(wlanhandle, 
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
	std::cout << "Available WLAN networks retrieved successfully!" << std::endl;

	for (int i = 0; i < pNetworkList->dwNumberOfItems; i++) {
		WLAN_AVAILABLE_NETWORK network = pNetworkList->Network[i];

		std::cout << "Signal quality: " << network.wlanSignalQuality << std::endl;
		std::cout << "SSID Length: " << network.dot11Ssid.uSSIDLength << std::endl;
		for (int j = 0; j < network.dot11Ssid.uSSIDLength; j++) {
			std::cout << network.dot11Ssid.ucSSID[j];

		}
 	}
	
	if (pInterfaceList != nullptr) {
		WlanFreeMemory(pInterfaceList);
		pInterfaceList = nullptr;
	}

	if (wlanhandle != nullptr) {
		WlanCloseHandle(wlanhandle, nullptr);
		wlanhandle = nullptr;
	}

	if (pNetworkList != nullptr) {
		WlanFreeMemory(pNetworkList);
		pNetworkList = nullptr;
	}

	return 0;
}