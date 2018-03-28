typedef struct street_light {
    int led_port, ir_port;
} street_light;

int light_intensity = 10;
int no_of_pole = 2;
int buzzer_pin = 2;

street_light st[] = { { 11, 12 }, {9, 10} };

int ldr = A0;
