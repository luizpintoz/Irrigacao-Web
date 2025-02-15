#ifndef DISPLAY_H
#define DISPLAY_H

void clearDisplay(uint8_t *ssd, struct render_area *frame_area);
void showMessage(char **msg, int msg_count, uint8_t *ssd, struct render_area *frame_area, float temperatura, float umidade);
void setup_display();
void clearDisplay(uint8_t *ssd, struct render_area *frame_area);

#endif