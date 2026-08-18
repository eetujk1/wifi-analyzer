# wifi-analyzer


A C++ application for analyzing nearby Wi-Fi networks on Windows using the Windows Native Wi-Fi API.

### Features
- Detect available Wi-Fi interfaces
- Scan for nearby Wi-Fi networks
- Display Wi-Fi network information
- Analyze signal strength and channels

### Technologies
- C++
- Windows Native Wi-Fi API
- CMake

### Project Status

🚧 In development

The project is currently in the early development stage. The initial Windows WLAN API integration has been implemented. The application can currently:

- Open a WLAN API client handle with `WlanOpenHandle()`
- Enumerate available WLAN interfaces with `WlanEnumInterfaces()`
- Start a WLAN scan with `WlanScan()`
- Retrieve available networks with `WlanGetAvailableNetworkList()`
- Display detected network SSIDs
- Display signal quality
- Calculate an estimated RSSI value from the signal quality

