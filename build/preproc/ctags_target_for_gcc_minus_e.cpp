# 1 "c:\\Users\\rajarshi\\Desktop\\audino\\highway.ino"
# 1 "c:\\Users\\rajarshi\\Desktop\\audino\\highway.ino"
# 2 "c:\\Users\\rajarshi\\Desktop\\audino\\highway.ino" 2

void setup()
{
    pinMode(8, 0x1);
    pinMode(3, 0x0);
    pinMode(10, 0x1);
    pinMode(13, 0x1);
    Serial.begin(9600);
    digitalWrite(8, 0x1);
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
        if (!ended && digitalRead(s.ir_port) != 0x1) {
            if (noise > 1000) {
                end_time = millis();
                ended = true;
            } else {
                noise++;
            }
        } else if (digitalRead(s.ir_port) == 0x1) {
            noise = 0;
        }
    }
    while (!ended) {
        if (!ended && digitalRead(s.ir_port) != 0x1) {
            if (noise > 1000) {
                end_time = millis();
                ended = true;
            } else {
                noise++;
            }
        } else if (digitalRead(s.ir_port) == 0x1) {
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
        digitalWrite(13, 0x1);
        delay(250);
        digitalWrite(13, 0x0);
    }
}

void loop()
{

    for (int pole_no = 0; pole_no < 1; pole_no++) {
        if (digitalRead(st[pole_no].ir_port) == 0x1) {
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
