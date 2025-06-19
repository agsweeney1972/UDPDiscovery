#!/usr/bin/env python3
# udp_responder.py
import socket

def get_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # Doesn't have to be reachable
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
    except Exception:
        ip = "127.0.0.1"
    finally:
        s.close()
    return ip

UDP_IP = ""
UDP_PORT = 50000

device_ip = get_ip()
device_hostname = socket.gethostname()
RESPONSE = ("SERVER FOUND:{};IP:{}".format(device_hostname, device_ip)).encode()

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024)
    if data == b"DISCOVER":
        sock.sendto(RESPONSE, addr)
