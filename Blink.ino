
void setup()
{
    pinMode(8, OUTPUT);
    pinMode(3, INPUT);
    pinMode(10, OUTPUT);
    pinMode(13, OUTPUT);
    Serial.begin(9600);
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
        counted = false;
        digitalWrite(13, LOW);
        count = 0;
        Serial.println("54");
    }
    else if (counted)
    {
        counted = false;
        count = 0;
    }

    if (digitalRead(3) == HIGH)
    {
        if (!i)
        {
            for (int j = 10; j <= 255; j++)
            {
                analogWrite(10, j);
                delay(2);
                if (digitalRead(3) == HIGH)
                {
                    count++;
                }
            }
            while (digitalRead(3) == HIGH)
            {
                count++;
                if (count >= 300)
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
            for (int j = 254; j >= 10; j--)
            {
                analogWrite(10, j);
                delay(6);
            }
            i = false;
        }
        analogWrite(10, 10);
    }
}