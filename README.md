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

- [x] Open a WLAN API client handle with `WlanOpenHandle()`
- [x] Enumerate available WLAN interfaces with `WlanEnumInterfaces()`
- [x] Start a WLAN scan with `WlanScan()`
- [x] Retrieve available networks with `WlanGetAvailableNetworkList()`
- [x] Display detected network SSIDs
- [x] Show estimated RSSI value in dBm
- [x] Determine WLAN channels
- [x] Determine WLAN frequencies in MHz


