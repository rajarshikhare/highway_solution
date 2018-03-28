#include "lights.h"

#include "RunningMedian.h"
#include "RunningAverage.h"

RunningAverage samples1 = RunningAverage(1000);
RunningMedian samples = RunningMedian(1000);

void setup() {
    for (int i = 0; i < no_of_pole; i++) {
        pinMode(st[i].led_port, OUTPUT);
        pinMode(st[i].ir_port, INPUT);
    }
    pinMode(buzzer_pin, OUTPUT);
    pinMode(ldr, OUTPUT);
    Serial.begin(9600);
}

bool carWent[] = {false, false};
unsigned long start_time[2] = {0, 0};

/*void slowON(street_light s) {
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
}*/

void slowDim(street_light s, int dim_intensity) {
    for (int i = 255; i >= dim_intensity; i--) {
        analogWrite(s.led_port, i);
        delay(2);
    }
}

void start(street_light s) { analogWrite(s.led_port, 255); }

void dim(street_light s) { analogWrite(s.led_port, light_intensity); }

void buzz(unsigned long t1, unsigned long t2) {
    if(t2 - t1 < 150){
      digitalWrite(buzzer_pin, HIGH);
      delay(250);
    }
    digitalWrite(buzzer_pin, LOW);
}

void loop() {
    int sensorValue = analogRead(ldr);
    samples.add(sensorValue);
    int light_intensity =  50 - map(samples.getMedian(), 0, 5, 0, 255);
    if(light_intensity < 0) light_intensity = 0;
    samples1.addValue(light_intensity);
    light_intensity = samples1.getAverage();
    Serial.println(light_intensity);
    for (int pole_no = 0; pole_no < no_of_pole; pole_no++) {
        if (digitalRead(st[pole_no].ir_port) == LOW) {
            if (!carWent[pole_no] && light_intensity > 0) {
                start_time[pole_no] = millis();
                start(st[pole_no]);
                carWent[pole_no] = true;
                Serial.println(start_time[pole_no]);
            }
        } else {
            if (carWent[pole_no]) {
                buzz(start_time[0], start_time[1]);
                dim(st[pole_no]);
                carWent[pole_no] = false;
                
            }
            analogWrite(st[pole_no].led_port, light_intensity);
        }
    }
}
