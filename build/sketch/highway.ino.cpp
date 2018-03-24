#include <Arduino.h>
#line 1 "c:\\Users\\rajarshi\\Desktop\\audino\\highway.ino"
#line 1 "c:\\Users\\rajarshi\\Desktop\\audino\\highway.ino"
#include "lights.h"

#line 3 "c:\\Users\\rajarshi\\Desktop\\audino\\highway.ino"
void setup();
#line 19 "c:\\Users\\rajarshi\\Desktop\\audino\\highway.ino"
void slowON(street_light s);
#line 58 "c:\\Users\\rajarshi\\Desktop\\audino\\highway.ino"
void slowDim(street_light s, int dim_intensity);
#line 66 "c:\\Users\\rajarshi\\Desktop\\audino\\highway.ino"
void buzz();
#line 79 "c:\\Users\\rajarshi\\Desktop\\audino\\highway.ino"
void loop();
#line 3 "c:\\Users\\rajarshi\\Desktop\\audino\\highway.ino"
void setup()
{
    pinMode(8, OUTPUT);
    pinMode(3, INPUT);
    pinMode(10, OUTPUT);
    pinMode(13, OUTPUT);
    Serial.begin(9600);
    digitalWrite(8, HIGH);
}

bool carWent = false;
unsigned long start_time = 0;
unsigned long end_time = 0;

int light_intensity = 1;

void slowON(street_light s)
{
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
        if (!ended && digitalRead(s.ir_port) != HIGH) {
            if (noise > 1000) {
                end_time = millis();
                ended = true;
            } else {
                noise++;
            }
        } else if (digitalRead(s.ir_port) == HIGH) {
            noise = 0;
        }
    }
    while (!ended) {
        if (!ended && digitalRead(s.ir_port) != HIGH) {
            if (noise > 1000) {
                end_time = millis();
                ended = true;
            } else {
                noise++;
            }
        } else if (digitalRead(s.ir_port) == HIGH) {
            noise = 0;
        }
    }
}

void slowDim(street_light s, int dim_intensity)
{
    for (int i = 255; i >= dim_intensity; i--) {
        analogWrite(s.led_port, i);
        delay(2);
    }
}

void buzz()
{
    int diff = end_time - start_time;

    Serial.print((float)70 / diff);
    Serial.println("m/s");
    if (diff < 250) {
        digitalWrite(13, HIGH);
        delay(250);
        digitalWrite(13, LOW);
    }
}

void loop()
{
    
    for (int pole_no = 0; pole_no < 1; pole_no++) {
        if (digitalRead(st[pole_no].ir_port) == HIGH) {
            if (!carWent) {
                start_time = millis();
                slowON(st[pole_no]);
                carWent = true;
            }
        } else {
            if (carWent) {
                buzz();
                slowDim(st[pole_no], light_intensity);
                carWent = false;
            }
            analogWrite(st[pole_no].led_port, light_intensity);
        }
    }
}
