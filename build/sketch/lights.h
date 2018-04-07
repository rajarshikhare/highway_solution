typedef struct street_light {
    int led_port, ir_port;
} street_light;

int light_intensity = 10;
int no_of_pole = 4;
int buzzer_pin = 8;

street_light st[] = { { 9, 13 }, {6, 12}, {3, 11}, {5, 10}};

int ldr = A0;

bool carWent[] = {false, false, false, false};
unsigned long start_time[] = {0, 0, 0, 0};
int car_count[] = {0, 0, 0, 0};


RunningAverage lightIntensities = RunningAverage(200);
RunningMedian ldrValues = RunningMedian(200);


RunningMedian ir[] = {RunningMedian(100), RunningMedian(100), RunningMedian(100), RunningMedian(100)};


unsigned long start_time_ir[] = {0, 0, 0, 0};