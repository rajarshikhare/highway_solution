# 1 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
# 1 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino"
# 2 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino" 2
# 3 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino" 2
# 4 "c:\\Users\\rajarshi\\Desktop\\New folder\\Blink_5\\highway\\highway.ino" 2


void setup() {
    for (int i = 0; i < no_of_pole; i++) {
        pinMode(st[i].led_port, 0x1);
        pinMode(st[i].ir_port, 0x0);
    }
    pinMode(buzzer_pin, 0x1);
    pinMode(ldr, 0x1);
    Serial.begin(9600);
}

void start(street_light s) { analogWrite(s.led_port, 255); }

void dim(street_light s) { analogWrite(s.led_port, light_intensity); }

void speedBuzz(unsigned long t1, unsigned long t2) {
    if (t2 - t1 < 150) {
        digitalWrite(buzzer_pin, 0x1);
        delay(250);
    }
    digitalWrite(buzzer_pin, 0x0);
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
        if (digitalRead(st[pole_no].ir_port) == 0x0) {
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
