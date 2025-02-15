#ifndef DISPLAY_H
#define DISPLAY_H

extern void clearDisplay(uint8_t *ssd, struct render_area *frame_area);
extern void showMessage(char **msg, int msg_count, uint8_t *ssd, struct render_area *frame_area, float temperatura, float umidade);
extern void setup_display();

#endif