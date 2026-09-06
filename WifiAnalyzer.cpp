#include "WifiAnalyzer.h"
#include <windows.h>
#include <wlanapi.h>
#include <iostream>

using namespace std;

void scanNetworks(HANDLE wlanhandle, GUID interfaceGuid);

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

	while (true)
	{
		std::cout << "\nPress ENTER to scan, or Q to quit: ";

		char input;
		std::cin.get(input);

		if (input == 'q' || input == 'Q')
		{
			break;
		}

		scanNetworks(wlanhandle, interfaceGuid);
	}
	
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

	
void scanNetworks(HANDLE wlanhandle, GUID interfaceGuid)
{

	DWORD result = WlanScan(
			wlanhandle,
			&interfaceGuid,
			nullptr,
			nullptr,
			nullptr);

	if (result != ERROR_SUCCESS) {
		std::cout << "Error: Failed to start WLAN scan. Code: "
		<< result << std::endl;
		return;

		}


	std::cout << "WLAN scan started successfully!" << std::endl;

	Sleep(3000);

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
		return;
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

				std::cout << "BSSID: ";

				for (int j = 0; j < 6; j++) {
					printf("%02X", bssEntry.dot11Bssid[j]);

					if (j < 5) {
						printf(":");
					}
				}

				std::cout << std::endl;
				std::cout << "RSSI: " << bssEntry.lRssi << " dBm" << std::endl;

				std::cout << "Frequency: " << bssEntry.ulChCenterFrequency / 1000
					<< " MHz" << std::endl;
			}

			std::cout << "-----------------------------------" << std::endl;

	}

	if (pBssList != nullptr)
	{
		WlanFreeMemory(pBssList);
		pBssList = nullptr;
	}

	if (pNetworkList != nullptr)
	{
		WlanFreeMemory(pNetworkList);
		pNetworkList = nullptr;
	}
}
