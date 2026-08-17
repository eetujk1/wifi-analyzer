
#include "WifiAnalyzer.h"
#include <windows.h>
#include <wlanapi.h>

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

	return 0;
}
