typedef struct street_light {
    int led_port, ir_port;
} street_light;

int light_intensity = 1;
int no_of_pole = 1
int buzzer_pin = 13;

street_light st[] = { { 10, 3 } };