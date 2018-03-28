#include "lights.h"

void setup() {
    for (int i = 0; i < no_of_pole; i++) {
        pinMode(st[i].led_port, OUTPUT);
        pinMode(st[i].ir_port, INPUT);
    }
    pinMode(buzzer_pin, OUTPUT);
    digitalWrite(buzzer_pin, LOW);
    pinMode(ldr, OUTPUT);
    Serial.begin(9600);
}

bool carWent[] = {false, false};
unsigned long start_time = 0;
unsigned long end_time = 0;

void slowON(street_light s) {
    int i = light_intensity;
    unsigned long s_tmp_time = millis();
    bool ended = false;
    int noise = 0;

    while (i < 255) {
        int diff = millis() - s_tmp_time;
        if (diff > 2) {
            analogWrite(s.led_port, i);
            s_tmp_time = millis();
            i++;
        }
        if (!ended && !digitalRead(s.ir_port) != HIGH) {
            if (noise > 1000) {
                end_time = millis();
                ended = true;
            } else {
                noise++;
            }
        } else if (!digitalRead(s.ir_port) == HIGH) {
            noise = 0;
        }
    }
    while (!ended) {
        if (!ended && !digitalRead(s.ir_port) != HIGH) {
            if (noise > 1000) {
                end_time = millis();
                ended = true;
            } else {
                noise++;
            }
        } else if (!digitalRead(s.ir_port) == HIGH) {
            noise = 0;
        }
    }
}

void slowDim(street_light s, int dim_intensity) {
    for (int i = 255; i >= dim_intensity; i--) {
        analogWrite(s.led_port, i);
        delay(2);
    }
}

void start(street_light s) { analogWrite(s.led_port, 255); }

void dim(street_light s) { analogWrite(s.led_port, 10); }

void buzz() {
    int diff = end_time - start_time;

    if (diff < 250) {
        digitalWrite(buzzer_pin, HIGH);
        delay(250);
        digitalWrite(buzzer_pin, LOW);
    }
}

void loop() {
    int sensorValue = analogRead(ldr);
    int light_intensity =  255 - map(sensorValue, 0, 2, 0, 255);
    for (int pole_no = 0; pole_no < no_of_pole; pole_no++) {
        if (digitalRead(st[pole_no].ir_port) == LOW) {
            if (!carWent[pole_no]) {
                start_time = millis();
                start(st[pole_no]);
                carWent[pole_no] = true;
            }
        } else {
            if (carWent[pole_no]) {
                // buzz();
                dim(st[pole_no]);
                carWent[pole_no] = false;
            }
            analogWrite(st[pole_no].led_port, light_intensity);
        }
    }
}