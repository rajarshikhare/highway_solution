
void setup()
{
    pinMode(8, OUTPUT);
    pinMode(3, INPUT);
    pinMode(10, OUTPUT);
    pinMode(13, OUTPUT);
    Serial.begin(9600);
}

 int slowON(int pin){
     int count = 0;
    for(int i = 0; i < 255; i++){
        analogWrite(pin, i);
        delay(2);
        if (digitalRead(3) == HIGH)
        {
            count++;
        }
    }
    return count;
}

void slowDim(int pin, int dim_intensity){
    for(int i = 255; i >= dim_intensity; i--){
        analogWrite(pin, i);
        delay(5);
    }
}

bool i = false;
int count = 0;
bool counted = false;



void loop()
{
    digitalWrite(8, HIGH);
    if (counted && count <= 80)
    {
        digitalWrite(13, HIGH);
        delay(250);
        digitalWrite(13, LOW);
        counted = false;
        count = 0;
    }
    else if (counted)
    {
        Serial.println(count);
        counted = false;
        count = 0;
    }

    if (digitalRead(3) == HIGH)
    {
        if (!i)
        {
            count = slowON(10);
            while (digitalRead(3) == HIGH)
            {
                count++;
                if (count >= 200)
                {
                    digitalWrite(13, HIGH);
                }
                delay(500);
            }
            digitalWrite(13, LOW);
            counted = true;
            i = true;
        }
    }
    else
    {
        if (i)
        {
            slowDim(10, 10);
            i = false;
        }
        analogWrite(10, 10);
    }
}