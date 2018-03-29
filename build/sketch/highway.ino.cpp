#include <Arduino.h>
#line 1 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
#line 1 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
#include "RunningAverage.h"
#include "RunningMedian.h"
#include "lights.h"


#line 6 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void setup();
#line 16 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void start(street_light s);
#line 18 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void dim(street_light s);
#line 20 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void speedBuzz(unsigned long t1, unsigned long t2);
#line 28 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
int getLightIntensity();
#line 38 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void loop();
#line 6 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void setup() {
    for (int i = 0; i < no_of_pole; i++) {
        pinMode(st[i].led_port, OUTPUT);
        pinMode(st[i].ir_port, INPUT);
    }
    pinMode(buzzer_pin, OUTPUT);
    pinMode(ldr, OUTPUT);
    Serial.begin(9600);
}

void start(street_light s) { analogWrite(s.led_port, 255); }

void dim(street_light s) { analogWrite(s.led_port, light_intensity); }

void speedBuzz(unsigned long t1, unsigned long t2) {
    if (t2 - t1 < 150) {
        digitalWrite(buzzer_pin, HIGH);
        delay(250);
    }
    digitalWrite(buzzer_pin, LOW);
}

int getLightIntensity(){
    int sensorValue = analogRead(ldr);
    ldrValues.add(sensorValue);
    int light_intensity = 50 - map(ldrValues.getMedian(), 0, 5, 0, 255);
    if (light_intensity < 0) light_intensity = 0;
    lightIntensities.addValue(light_intensity);

    return lightIntensities.getAverage();
}

void loop() {
    light_intensity = getLightIntensity();
    for (int pole_no = 0; pole_no < no_of_pole; pole_no++) {
        if (digitalRead(st[pole_no].ir_port) == LOW) {
            if (!carWent[pole_no] && light_intensity > 0) {
                start_time[pole_no] = millis();
                start(st[pole_no]);
                carWent[pole_no] = true;
            }
        } else {
            if (carWent[pole_no]) {
                speedBuzz(start_time[0], start_time[1]);
                dim(st[pole_no]);
                carWent[pole_no] = false;
            }
            analogWrite(st[pole_no].led_port, light_intensity);
        }
    }
}

