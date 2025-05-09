import network
import socket
from machine import Pin
import time

# Configura el pin donde se conectará el servo o LED
puerta = Pin(2, Pin.OUT)  # GPIO2 (puedes usar otro)

# Conectar a WiFi
def conectar_wifi(ssid, password):
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    print("Conectando a WiFi...", end="")
    while not wlan.isconnected():
        time.sleep(1)
        print(".", end="")
    print("\nConectado. IP:", wlan.ifconfig()[0])
    return wlan.ifconfig()[0]

# Acción para abrir la puerta (simulación)
def abrir_puerta():
    print("🟢 ¡Puerta abierta!")
    puerta.on()
    time.sleep(3)  # Simula el tiempo de apertura
    puerta.off()
    print("🔴 Puerta cerrada")

# Inicia servidor web
def servidor_http():
    addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
    s = socket.socket()
    s.bind(addr)
    s.listen(1)
    print("Servidor web listo...")

    while True:
        cl, addr = s.accept()
        print('Cliente conectado desde', addr)
        request = cl.recv(1024)
        request_str = request.decode()

        if 'GET /abrir' in request_str:
            abrir_puerta()
            response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nPuerta abierta"
        else:
            response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nComando no válido"
        
        cl.send(response)
        cl.close()

# Configura tu red WiFi
SSID = "Tu_SSID"
PASSWORD = "Tu_PASSWORD"

try:
    ip = conectar_wifi(SSID, PASSWORD)
    servidor_http()
except Exception as e:
    print("Error:", e)