#include <Arduino.h>
#line 1 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
#line 1 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
#include <SoftwareSerial.h>
#include "RunningAverage.h"
#include "RunningMedian.h"
#include "lights.h"

SoftwareSerial BTserial(0, 1);  // RX | TX

#line 8 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void setup();
#line 19 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void start_led(street_light s, int pole_no);
#line 28 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void dim(street_light s);
#line 30 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void speedBuzz(int pole_no);
#line 53 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
int getLightIntensity();
#line 63 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void sendData();
#line 88 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
int getIrVal(int pole_no);
#line 98 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void loop();
#line 8 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
void setup() {
    for (int i = 0; i < no_of_pole; i++) {
        pinMode(st[i].led_port, OUTPUT);
        pinMode(st[i].ir_port, INPUT);
    }
    pinMode(buzzer_pin, OUTPUT);
    pinMode(ldr, OUTPUT);
    Serial.begin(9600);
    //BTserial.begin(9600);
}

void start_led(street_light s, int pole_no) {
    analogWrite(s.led_port, 255);
    //car_count[pole_no]++;
    if(millis() - start_time_ir[pole_no] > 1000){
        car_count[pole_no]++;
        start_time_ir[pole_no] = millis();
    }
}

void dim(street_light s) { analogWrite(s.led_port, light_intensity); }

void speedBuzz(int pole_no) {
    if(pole_no != 0){
        car_count[pole_no - 1] = (car_count[pole_no - 1] > 0) ? car_count[pole_no - 1] - 1 : 0; 
    }
    sendData();
    if (pole_no == 0) {
        return;
    }
    int t2 = start_time[pole_no];
    int t1 = start_time[pole_no - 1];
    if (t2 - t1 <= 0) return;
    float speed_ = (float)90 / (float)((t2 - t1));
    //Serial.print("speed: ");
    //Serial.print(speed);
    //Serial.println("m/s");
    speed[pole_no - 1 ] = speed_;
    if (speed_ > 1.0) {
        digitalWrite(buzzer_pin, HIGH);
        delay(250);
    }
    digitalWrite(buzzer_pin, LOW);
}

int getLightIntensity() {
    int sensorValue = analogRead(ldr);
    ldrValues.add(sensorValue);
    int light_intensity = 60 - map(ldrValues.getMedian(), 0, 5, 0, 255);
    if (light_intensity < 0) light_intensity = 0;
    lightIntensities.addValue(light_intensity);

    return lightIntensities.getAverage();
}

void sendData() {
    /*BTserial.print(car_count[0]);
    BTserial.print(","); 
    BTserial.print(car_count[1]);
    BTserial.print(","); 
    BTserial.print(car_count[2]);
    BTserial.print(","); 
    BTserial.print(car_count[3]);
    BTserial.print(";"); */
    Serial.print(speed[0]);
    Serial.print("+");
    Serial.print(speed[1]);
    Serial.print("+");
    Serial.print(speed[2]);
    Serial.print("+");
    Serial.print(car_count[0]);
    Serial.print("+");
    Serial.print(car_count[1]);
    Serial.print("+");
    Serial.print(car_count[2]);
    /*Serial.print("+");
    Serial.print(car_count[3]);*/
    Serial.println("");
}

int getIrVal(int pole_no){
    /*if(millis() - start_time_ir[pole_no] > 100){
        start_time_ir[pole_no] = millis();
        ir[pole_no].clear();
    }*/
    ir[pole_no].addValue(digitalRead(st[pole_no].ir_port));
    int k =  ir[pole_no].getAverage();
    return k;
}

void loop() {
    //sendData();
    light_intensity = getLightIntensity();
    for (int pole_no = 0; pole_no < no_of_pole; pole_no++) {
        if (digitalRead(st[pole_no].ir_port) == LOW) {
        //if (getIrVal(pole_no) == LOW) {
            if (!carWent[pole_no] && light_intensity > 0) {
                start_time[pole_no] = millis();
                start_led(st[pole_no], pole_no);
                carWent[pole_no] = true;
            }
        } else {
            if (carWent[pole_no]) {
                speedBuzz(pole_no);
                dim(st[pole_no]);
                carWent[pole_no] = false;
            }
            analogWrite(st[pole_no].led_port, light_intensity);
        }
    }
}

