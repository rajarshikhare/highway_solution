
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
int count = 0;
bool counted = false;
unsigned long start_time = 0;
unsigned long end_time = 0;

void slowON(int pin)
{
    int i = 10;
    unsigned long s_tmp_time = millis();
    bool ended = false;
    int noise = 0;

    while (i < 255)
    {
        int diff = millis() - s_tmp_time;
        if (diff > 2)
        {
            analogWrite(pin, i);
            s_tmp_time = millis();
            i++;
        }
        if (!ended && digitalRead(3) != HIGH)
        {
            if (noise > 500)
            {
                end_time = millis();
                ended = true;
            }
            else
            {
                noise++;
            }
        }
        else if (digitalRead(3) == HIGH)
        {
            noise--;
        }
    }
    while (!ended)
    {
        if (!ended && digitalRead(3) != HIGH)
        {
            if (noise > 500)
            {
                end_time = millis();
                ended = true;
            }
            else
            {
                noise++;
            }
        }
        else if (digitalRead(3) == HIGH)
        {
            noise--;
        }
    }
}

void slowDim(int pin, int dim_intensity)
{
    for (int i = 255; i >= dim_intensity; i--)
    {
        analogWrite(pin, i);
        delay(5);
    }
}

void buzz()
{
    int diff = end_time - start_time;

    Serial.print((float)70/diff);
    Serial.println("m/s");
    if (diff < 250)
    {
        digitalWrite(13, HIGH);
        delay(250);
        digitalWrite(13, LOW);
    }
}

void loop()
{

    if (digitalRead(3) == HIGH)
    {
        if (!carWent)
        {
            start_time = millis();
            slowON(10);
            carWent = true;
        }
    }
    else
    {
        if (carWent)
        {
            buzz();
            slowDim(10, 10);
            carWent = false;
        }
        analogWrite(10, 10);
    }
}