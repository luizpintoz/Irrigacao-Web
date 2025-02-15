#include "ssd1306.h"
#include "hardware/i2c.h"
#include <string.h>
#include <stdio.h>

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

void clearDisplay(uint8_t *ssd, struct render_area *frame_area)
{
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, frame_area);
}

// Função para exibir mensagem no display com temperatura e umidade
void showMessage(char **msg, int msg_count, uint8_t *ssd, struct render_area *frame_area, float temperatura, float umidade)
{
    clearDisplay(ssd, frame_area); // Limpa o display

    int y = 0; // Posição vertical inicial

    // Exibe as mensagens do array
    for (int i = 0; i < msg_count; i++)
    {
        ssd1306_draw_string(ssd, 5, y, msg[i]); // Desenha a mensagem na posição (5, y)
        y += 8;                                 // Ajusta para a próxima linha (8 pixels de altura por linha)
    }

    // Exibe a temperatura e umidade em duas linhas separadas
    char temp_line[32];
    char umid_line[32];
    snprintf(temp_line, sizeof(temp_line), "Temp: %.1fC", temperatura);
    snprintf(umid_line, sizeof(umid_line), "Umi: %.1f%%", umidade);

    // Centraliza as linhas de temperatura e umidade
    ssd1306_draw_string(ssd, 5, y, temp_line); // Desenha a temperatura
    y += 8;                                    // Ajusta para a próxima linha
    ssd1306_draw_string(ssd, 5, y, umid_line); // Desenha a umidade

    render_on_display(ssd, frame_area); // Renderiza o buffer no display
}

void setup_display()
{
    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);

    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();
}