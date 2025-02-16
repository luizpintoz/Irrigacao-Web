#ifndef REQUEST_H
#define REQUEST_H

#define WIFI_SSID "Wireless"     // Altere para o nome da sua rede
#define WIFI_PASS ""             // Altere a senha da rede
#define SERVER_IP "192.168.1.20" // Altere para o IP do seu servidor Flask
#define SERVER_PORT 5000
#define RESPONSE_BUFFER_SIZE 1024
#define REQUEST_INTERVAL_MS 500 // Intervalo de 5 segundos entre requisições

static char response_buffer[RESPONSE_BUFFER_SIZE];
static int response_length = 0;

extern int extract_number_json();
extern int main_request();
extern void make_request();

#endif