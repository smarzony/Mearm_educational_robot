void test_pcf()
{
    for (uint8_t pcf_no = 0; pcf_no <= 1; pcf_no++)
    {
        for (uint8_t i = 0; i <= 7; i++)
        {
            if (pcf8574[pcf_no].digitalRead(i) == 0)
            {
                Serial.println("PCF " + String(pcf_no) + " input " + String(i) + ": LOW");
            }
            if (pcf8574[pcf_no].digitalRead(i) == 1)
            {
                // Serial.println("PCF " + String(pcf_no) + " input " + String(i) + ": HIGH");
            }
        }
    }
    // Serial.println();
    delay(500);
}

void test_servos()
{
    for (int pos = 204; pos < 408; pos++)
    {
        pwm.setPWM(SERVO_ROTATE, 0, pos);
        Serial.println(pos);
        delay(40);
    }
    Serial.println("Half sweep");
    for (int pos = 408; pos > 204; pos--)
    {
        pwm.setPWM(SERVO_ROTATE, 0, pos);
        Serial.println(pos);
        delay(40);
    }
    Serial.println("Full sweep");
}