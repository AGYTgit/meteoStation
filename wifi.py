import network
import socket

class WIFI:

    def __init__(self):
        self.ssid = 'ESP32_AP'
        self.password = '12345678'
        self.html_content = self.import_html()
        self.sock = None
        self.temp = 0.0
        self.humid = 0.0

    def create_ap(self):
        ap = network.WLAN(network.AP_IF)
        ap.active(True)
        ap.config(essid=self.ssid, password=self.password)
        ap.config(channel=6, max_clients=5)

        while not ap.active():
            pass

        print('Access Point created successfully!')
        print('Network details:', ap.ifconfig())

    def import_html(self):
        with open('index.html', 'r', encoding='utf-8') as file:
            return file.read()

    def send_data(self, temp, humid):
        print(f"Updated values -> Temp: {temp}, Humid: {humid}")
        self.temp = temp
        self.humid = humid

        self.check_connection()

    def http_server(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind(('', 80))
        self.sock.listen(5)
        print('HTTP server running...')

    def check_connection(self):
        client, addr = self.sock.accept()
        print(f'Client connected: {addr}')

        request = client.recv(1024)

        client.send('HTTP/1.1 200 OK\n')
        client.send('Content-Type: text/html\n')
        client.send('Connection: close\n\n')

        html_response = self.html_content.format(temp=self.temp, humid=self.humid)
        client.sendall(html_response.encode('utf-8'))
        client.close()

    def init(self):
        self.create_ap()
        self.http_server()

        print("Waiting for new data to send...")

