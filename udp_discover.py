#!/usr/bin/env python3
# vanfab_udp_discover.py
import socket

UDP_IP = "255.255.255.255"
UDP_PORT = 50000
MESSAGE = b"DISCOVER"

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
sock.settimeout(2)  # Set a timeout for the discovery process
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

try:
    while True:
        data, addr = sock.recvfrom(1024)
        print("Received from {}: {}".format(addr, data.decode()))
except socket.timeout:
    print("Discovery finished.")
