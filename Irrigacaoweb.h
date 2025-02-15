#ifndef IRRIGACAO_WEB_H
#define IRRIGACAO_WEB_H

#define LED_R 13                // Led Bomba desligada
#define LED_G 11                // Led Bomba ligada
#define BUZZER_PIN 21           // Pino do buzzer
#define REQUEST_INTERVAL_MS 500 // Intervalo de 500ms entre requisições

static char *bombaLigada[] = {
    "              ",
    " BOMBA LIGADA ",
    "              "};

static char *bombaDesligada[] = {
    "                ",
    "BOMBA DESLIGADA ",
    "                "};

#endif