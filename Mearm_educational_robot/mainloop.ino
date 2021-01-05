void mainloop()
{
	

	if (now - last_save_time > 1000)
	{
		last_save_time = now;
		save_coords(positions, 0);
	}

	read_buttons();
	step_mode_switch_pcf(PCF1, BUTTON_STEP_MODE_PCF1);
	if (read_memory)
	{
		memory_read();
		read_memory = false;
	}

	if (saving)
	{
		save_step(address_for_save, true);
		saving = false;
	}

	if (program_run)
	{
		auto_mode();
	}

	if (erasing)
	{
		erase_program(0);
		address_for_save = 1;
		Serial.println("Program erased");
		erasing = false;
	}

	if (program_run == false)
	{
		manual_mode();
	}
	/*
	if (pcf8574[PCF1].digitalRead(0) == LOW)
	{
		delay(300);
		if (pcf8574[PCF1].digitalRead(0) == LOW)
			positions.SERVO_MOVE_SPEED++;
			if (positions.SERVO_MOVE_SPEED > 3)
				positions.SERVO_MOVE_SPEED = 0;
			Serial.print("Servo move speed: ");
			Serial.println(positions.SERVO_MOVE_SPEED);		
	}
	
	read_button_inc_switch_pcf2(PCF1, BUTTON_SPEED_SELECT_PCF1, 0, 3, positions.SERVO_MOVE_SPEED, 50);
	*/	
}