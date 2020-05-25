bool twoButtonControl(uint16_t &value, byte button_minus, byte button_plus, uint8_t SERVO_MOVE_SPEED)
{
	byte step_size = 0;
	//  switch (positions.SERVO_MOVE_SPEED)
	switch (SERVO_MOVE_SPEED)
	{
	case 0:
		step_size = 1;
		break;
	case 1:
		step_size = 2;
		break;
	case 2:
		step_size = 5;
		break;
	case 3:
		step_size = 10;
		break;
	}

	if (digitalRead(button_minus) == 0)
	{
		delay(50);
		if (digitalRead(button_minus) == 0)
			value -= step_size;
		return true;
	}
	else if (digitalRead(button_plus) == 0)
	{
		delay(50);
		if (digitalRead(button_plus) == 0)
			value += step_size;
		return true;
	}
	return false;
}

bool twoButtonControl_pcf(uint16_t &value, byte button_minus, byte button_plus, uint8_t SERVO_MOVE_SPEED, uint8_t module_no)
{
	byte step_size = 0;
	//  switch (positions.SERVO_MOVE_SPEED)
	switch (SERVO_MOVE_SPEED)
	{
	case 0:
		step_size = 1;
		break;
	case 1:
		step_size = 2;
		break;
	case 2:
		step_size = 5;
		break;
	case 3:
		step_size = 10;
		break;
	}

	if (pcf8574[module_no].digitalRead(button_minus) == 0)
	{
		delay(50);
		if (pcf8574[module_no].digitalRead(button_minus) == 0)
			value -= step_size;
		return true;
	}
	else if (pcf8574[module_no].digitalRead(button_plus) == 0)
	{
		delay(50);
		if (pcf8574[module_no].digitalRead(button_plus) == 0)
			value += step_size;
		return true;
	}
	return false;
}

bool twoButtonControl_pcf_main(uint16_t &value, byte button_minus, byte button_plus, uint8_t SERVO_MOVE_SPEED, uint8_t module_no)
{
	byte step_size = 0;
	//  switch (positions.SERVO_MOVE_SPEED)
	switch (SERVO_MOVE_SPEED)
	{
	case 0:
		step_size = 1;
		break;
	case 1:
		step_size = 2;
		break;
	case 2:
		step_size = 5;
		break;
	case 3:
		step_size = 10;
		break;
	}

	if (pcf8574[module_no].digitalRead(button_minus) == 0)
	{
		delay(50);
		if (pcf8574[module_no].digitalRead(button_minus) == 0)
			value -= step_size;
		return true;
	}
	else if (digitalRead(button_plus) == 0)
	{
		delay(50);
		if (digitalRead(button_plus) == 0)
			value += step_size;
		return true;
	}
	return false;
}

void read_button_inc_switch(byte button, int limit_min, int limit_max, uint8_t& state)
{
	static unsigned long lastTime;
	unsigned long timeNow = millis();

	if (digitalRead(button) == 0) {
		if (timeNow - lastTime < BUTTON_DELAY)
			return;
		if (digitalRead(button) == 0)
		{
			state += 1;
			if (state <= limit_max)
			{
				//        Serial.print("Inc switch: ");
				//        Serial.println(state);
			}
		}
		lastTime = timeNow;
	}
	if (state > limit_max)
	{
		state = limit_min;
		//    Serial.print("Inc switch: ");
		//    Serial.println(state);
	}
	if (state < limit_min)
	{
		state = limit_max;
		//    Serial.print("Inc switch: ");
		//    Serial.println(state);
	}
}

void read_button_inc_switch_pcf(uint8_t pcf_module, uint8_t button, int limit_min, int limit_max, uint8_t& state, unsigned long long lastTime)
{
	//static unsigned long lastTime;
	unsigned long timeNow = millis();

	if (pcf8574[pcf_module].digitalRead(button) == LOW) {
		if (timeNow - lastTime < BUTTON_DELAY)
			return;
		if (pcf8574[pcf_module].digitalRead(button) == LOW)
		{
			state += 1;
			if (state <= limit_max)
			{
				        Serial.print("Inc switch: ");
				        Serial.println(state);
			}
		}
		lastTime = timeNow;
	}
	if (state > limit_max)
	{
		state = limit_min;
		    Serial.print("Inc switch: ");
		    Serial.println(state);
	}
	if (state < limit_min)
	{
		state = limit_max;
		    Serial.print("Inc switch: ");
		    Serial.println(state);
	}
}

void read_button_inc_switch_pcf2(uint8_t pcf_module, uint8_t pin, int8_t limit_down, int8_t limit_up, uint8_t &value, uint16_t delay_time)
{
	if (pcf8574[pcf_module].digitalRead(pin) == LOW)
	{
		delay(delay_time);
		if (pcf8574[pcf_module].digitalRead(pin) == LOW)
		{
			value++;
			if (value > limit_up)
				value = limit_down;
			//Serial.print("value: ");
			//Serial.println(value);
		}
	}
}

void read_button_neg_switch(byte button, bool &state)//, unsigned long &lastTime)
{
	static unsigned long lastTime;
	unsigned long timeNow = millis();

	if (digitalRead(button) == 0) {
		if (timeNow - lastTime < BUTTON_DELAY)
			return;
		if (digitalRead(button) == 0)
		{
			state = !state;
		}
		lastTime = timeNow;
	}
}

void read_button_neg_switch_pcf(uint8_t pcf_module, uint8_t button, bool &state)//, unsigned long &lastTime)
{
	static unsigned long lastTime;
	unsigned long timeNow = millis();

	if (pcf8574[pcf_module].digitalRead(button) == 0) {
		if (timeNow - lastTime < BUTTON_DELAY)
			return;
		if (pcf8574[pcf_module].digitalRead(button) == 0)
		{
			state = !state;
		}
		lastTime = timeNow;
	}
}

void read_button_neg_switch_pcf2(uint8_t pcf_module, uint8_t pin, bool &state, uint16_t delay_time)//, unsigned long &lastTime)
{
	if (pcf8574[pcf_module].digitalRead(pin) == LOW)
	{
		delay(delay_time);
		if (pcf8574[pcf_module].digitalRead(pin) == LOW)
		{
			state = !state;
		}
	}
}

void rotoryEncoder_init(uint8_t PIN_CLK, uint8_t PIN_DT)
{
	pinMode(PIN_CLK, INPUT);
	pinMode(PIN_DT, INPUT);
}

void rotoryEncoder_get_value(uint8_t PIN_CLK, uint8_t PIN_DT, bool & PIN_CLK_Last, uint16_t & value)
{
	bool n = digitalRead(PIN_CLK);
	if ((PIN_CLK_Last == LOW) && (n == HIGH)) {
		if (digitalRead(PIN_DT) == LOW) {
			value--;
		}
		else {
			value++;
		}
		Serial.print(value);
		Serial.print("/");
	}
	PIN_CLK_Last = n;
}

void buttons_init()
{
	pcf8574[0].begin();
	pcf8574[1].begin();

	for (int x = 0; x <= 1; x++)
	{
		//pcf8574[x].begin();
		for (int y = 0; y <= 7; y++)
		{
			pcf8574[x].pinMode(y, INPUT_PULLUP);
		}		
	}


	//pcf8574.pinMode(ROTORY_ENCODER_CLK, INPUT);
	//pcf8574.pinMode(ROTORY_ENCODER_DT, INPUT);

	for (int i = 2; i <= 6; i++)
	{
		pinMode(i, INPUT_PULLUP);
	}
	delay(10);

	//pinMode(BUTTON_STEP_MODE_MAIN, INPUT_PULLUP);
	//pinMode(BUTTON_STEP_BACK_MAIN, INPUT_PULLUP);
	//pinMode(BUTTON_EXIT_PROGRAM_MAIN, INPUT_PULLUP);
	//  pinMode(ROTORY_ENCODER_CLK, INPUT_PULLUP);
	//  pinMode(ROTORY_ENCODER_DT, INPUT_PULLUP);
	//  pinMode(ROTORY_ENCODER_PUSHBUTTON, INPUT_PULLUP);
}

void read_buttons()
{
	read_button_neg_switch_pcf2(PCF1, BUTTON_SAVE_POSITION_PCF1, saving, BUTTON_DELAY);
	read_button_neg_switch_pcf2(PCF1, BUTTON_ERASE_PROGRAM_PCF1, erasing, BUTTON_DELAY);
	read_button_neg_switch_pcf2(PCF1, BUTTON_RUN_PROGRAM_PCF1, program_run, BUTTON_DELAY);
	read_button_neg_switch_pcf2(PCF1, BUTTON_STEP_BACK_PCF1, read_memory, BUTTON_DELAY);
	read_button_neg_switch_pcf2(PCF1, BUTTON_STEP_MODE_PCF1, step_mode, BUTTON_DELAY);
	//step_mode_switch();
}


void step_mode_switch_pcf(uint8_t pcf_module, uint8_t pin)
{
	int old_step_mode = step_mode;
	read_button_neg_switch_pcf2(pcf_module, pin, step_mode, BUTTON_DELAY);
	if (old_step_mode != step_mode)
	{
		Serial.print("Step mode ");
		if (step_mode == true)
			Serial.println("ON");
		else
			Serial.println("OFF");
	}
}
