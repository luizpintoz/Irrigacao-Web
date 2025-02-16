#ifndef SEND_DATA_H
#define SEND_DATA_H

static const char *API_KEY = ""; // Altere para sua API
static char thingspeak_response[512] = "Aguardando resposta do servidor...";

extern void send_data_to_thingspeak(float temperatura, float umidade, int estado_bomba);

#endif