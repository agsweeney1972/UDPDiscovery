# UDP Device Discovery Suite

## Overview
This project provides a cross-platform solution for discovering headless devices (such as Dell Edge Gateway 3001/3002 units) on a local network. It is designed to help administrators and integrators quickly identify the IP addresses and hostnames of devices that do not have displays or direct user interfaces, enabling remote access and management.

## Components
- **udp_responder.py**: Python responder to be run on each device to be discovered. Listens for UDP discovery packets and responds with its hostname and IP address.
- **udp_discover.py**: Python client for sending a UDP broadcast discovery packet and printing responses from devices running the responder.
- **udp_discover.cpp / udp_discover.exe**: C++/Windows version of the discovery client for fast device discovery from Windows systems.
- **snapcraft.yaml**: Snap packaging for deploying the responder as a service on Ubuntu Core and other snap-supported systems.

## How It Works
1. The discovery client sends a UDP broadcast message ("DISCOVER") to port 50000 on the local network.
2. Any device running the responder on the same subnet receives the message and replies with its hostname and IP address.
3. The client prints out the responses, allowing identification of all active devices on the network segment.

## Example Output
```
Received from ('172.16.92.101', 50000): SERVER FOUND:edge-gateway-1;IP:172.16.92.101
Received from ('172.16.92.102', 50000): SERVER FOUND:edge-gateway-2;IP:172.16.92.102
```
Or, on Windows:
```
Received from 172.16.92.101: SERVER FOUND:edge-gateway-1;IP:172.16.92.101
Received from 172.16.92.102: SERVER FOUND:edge-gateway-2;IP:172.16.92.102
```

## Usage
### On Each Device (Responder)
- Install Python 3 if not already present.
- Run:
  ```sh
  python3 udp_responder.py
  ```
- Or, install as a snap for automatic startup on Ubuntu Core.

### On a Management Computer (Discoverer)
- To use the Python client:
  ```sh
  python3 udp_discover.py
  ```
- To use the Windows C++ client:
  - Compile with Visual Studio:
    ```sh
    cl /EHsc udp_discover.cpp ws2_32.lib
    ```
  - Or use the prebuilt `udp_discover.exe`.

## Limitations
- Only discovers devices on the same subnet (network segment).
- Devices with static IPs outside the subnet will not be found unless the management computer’s IP is temporarily changed to match.
- For devices with unknown or misconfigured IPs, use ARP scanning tools (e.g., arp-scan, Wireshark) to detect them by MAC address.
- The Windows .exe runs very slowly with Norton Antivirus - I created an exception in Norton settings which fixed it for me.

## License
MIT
