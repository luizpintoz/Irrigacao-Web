#ifndef BUZZER_H
#define BUZZER_H

// Configuração do pino do buzzer
#define BUZZER_PIN 21
// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY 500

void main_buzzer();
void pwm_init_buzzer(uint pin);
void beep(uint pin, uint duration_ms);

#endif