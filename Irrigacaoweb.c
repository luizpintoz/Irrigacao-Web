#include "request/request.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "display/ssd1306.h"
#include "display/display.h"
#include "buzzer/buzzer.h"
#include "hardware/pwm.h"
#include "thingspeak/send_data.h"

#define LED_R 13                // Led Bomba desligada
#define LED_G 11                // Led Bomba ligada
#define BUZZER_PIN 21           // Pino do buzzer
#define REQUEST_INTERVAL_MS 500 // Intervalo de 500ms entre requisições

char *bombaLigada[] = {
    "              ",
    " BOMBA LIGADA ",
    "              "};

char *bombaDesligada[] = {
    "                ",
    "BOMBA DESLIGADA ",
    "                "};

// Funções para simular temperatura e umidade
float simular_temperatura()
{
    // Simula uma temperatura entre 20 e 40 graus
    return 20.0 + (rand() % 200) / 10.0;
}

float simular_umidade()
{
    // Simula uma umidade entre 30% e 80%
    return 30.0 + (rand() % 500) / 10.0;
}

int main()
{
    stdio_init_all(); // Inicializa a saída padrão
    setup_display();
    pwm_init_buzzer(BUZZER_PIN); // Inicializa o buzzer

    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // Zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];

    clearDisplay(ssd, &frame_area);

    // Inicializa o Wi-Fi e faz a primeira requisição
    if (main_request() != 0)
    {
        printf("Erro ao inicializar a conexão Wi-Fi ou fazer a primeira requisição\n");
        return 1;
    }

    // Configura o LED como saída
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    // Simula temperatura e umidade
    float temperatura = simular_temperatura();
    float umidade = simular_umidade();
    int proxima_simulador = 0; // Inicializa o contador
    int estado_automatico = 0;
    int estado_bomba;
    printf("Temperatura: %.1f°C, Umidade: %.1f%%\n", temperatura, umidade);

    while (true)
    {
        proxima_simulador++;

        // Serve para gerar valores aleatórios a cada 10 iterações
        if (proxima_simulador == 10)
        {
            proxima_simulador = 0;
            temperatura = simular_temperatura();
            umidade = simular_umidade();
            printf("Temperatura: %.1f°C, Umidade: %.1f%%\n", temperatura, umidade);
        }

        // Faz uma requisição à API
        make_request();
        // Enviando os dados pra ThingSpeak
        send_data_to_thingspeak(temperatura, umidade, estado_bomba);

        // Processa a resposta da API
        int estado = extract_number_json(); // Estado manual (1 = ligado, 0 = desligado, -1 = automático)
        printf("Temperatura: %.1f°C, Umidade: %.1f%%\n", temperatura, umidade);

        // Lógica automática para ligar/desligar a bomba
        if (temperatura > 30.0 || umidade < 50.0) // Usa o operador OU (||) para qualquer uma das condições
        {
            estado_automatico = 1; // Liga a bomba automaticamente
        }
        else
        {
            estado_automatico = 0;
        }

        // Decide o estado da bomba (manual ou automático)
        if (estado == -1) // Modo automático
        {
            estado_bomba = estado_automatico;
            printf("Modo automático: %d\n", estado_bomba);
        }
        else // Modo manual
        {
            estado_bomba = estado;
            printf("Modo manual: %d\n", estado_bomba);
        }

        // Atualiza o estado do LED com base na resposta
        if (estado_bomba == 1)
        {
            gpio_put(LED_G, 1);                                                  // Liga o LED verde
            gpio_put(LED_R, 0);                                                  // Desliga o LED vermelho
            showMessage(bombaLigada, 3, ssd, &frame_area, temperatura, umidade); // Exibe "BOMBA LIGADA"
            beep(BUZZER_PIN, 200);                                               // Emite um beep de 200ms
        }
        else if (estado_bomba == 0)
        {
            gpio_put(LED_R, 1);                                                     // Liga o LED vermelho
            gpio_put(LED_G, 0);                                                     // Desliga o LED verde
            showMessage(bombaDesligada, 3, ssd, &frame_area, temperatura, umidade); // Exibe "BOMBA DESLIGADA"
            pwm_set_gpio_level(BUZZER_PIN, 0);                                      // Desliga o buzzer
        }

        // Aguarda um curto intervalo antes da próxima requisição
        sleep_ms(REQUEST_INTERVAL_MS);
    }

    cyw43_arch_deinit();
    return 0;
}