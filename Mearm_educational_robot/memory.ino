void save_step(uint16_t &address, bool with_end)
{
	if (address_for_save < MAX_PROGRAM_STEPS + 1)
	{
		save_coords(positions, address_for_save);
		servo_positions program_end;
		program_end.SERVO_ROTATE_POS = 1;
		program_end.SERVO_VERTICAL_POS = 2;
		program_end.SERVO_EXTEND_POS = 3;
		program_end.SERVO_GRIPPER_POS = 4;
		program_end.SERVO_MOVE_SPEED = 5;
		if (with_end == true)
			save_coords(program_end, address_for_save + 1);
		address++;
		Serial.println("Position saved");
	}
	else
		Serial.println("Program limit reached!");
}

void erase_program(uint16_t start_address)
{
	if (start_address <= 1)
		start_address = 1;
	if (start_address > MAX_PROGRAM_STEPS)
		Serial.println("Wrong erase address");
	servo_positions no_step;
	for (uint16_t addr = start_address; addr <= MAX_PROGRAM_STEPS + 1; addr++)
	{
		save_coords(no_step, addr);
	}
}

void save_coords(servo_positions positions, uint16_t address)
{
	uint16_t block_size = sizeof(servo_positions);
	uint16_t eeAddress = address * block_size;
	EEPROM.put(eeAddress, positions);
}

void get_coords(servo_positions &positions, uint16_t address)
{
	uint16_t block_size = sizeof(servo_positions);
	uint16_t eeAddress = address * block_size;
	EEPROM.get(eeAddress, positions);
}

void print_info(servo_positions positions)
{
	Serial.print("Pos: ");
	Serial.print(positions.SERVO_ROTATE_POS);
	Serial.print(" ");
	Serial.print(positions.SERVO_VERTICAL_POS);
	Serial.print(" ");
	Serial.print(positions.SERVO_EXTEND_POS);
	Serial.print(" ");
	Serial.print(positions.SERVO_GRIPPER_POS);
	Serial.print(" Speed: ");
	Serial.println(positions.SERVO_MOVE_SPEED);
}

void set_position(servo_positions positions)
{
	#ifdef SERVOS_WORKING
		pwm.setPWM(SERVO_ROTATE, 0, positions.SERVO_ROTATE_POS);
		pwm.setPWM(SERVO_VERTICAL, 0, positions.SERVO_VERTICAL_POS);
		pwm.setPWM(SERVO_EXTEND, 0, positions.SERVO_EXTEND_POS);
		pwm.setPWM(SERVO_GRIPPER, 0, positions.SERVO_GRIPPER_POS);
	#endif
}

void memory_read()
{
	servo_positions temp_pos;
	address_for_save = 0;
	for (uint16_t addr = 0; addr <= MAX_PROGRAM_STEPS + 1; addr++)
	{
		get_coords(temp_pos, addr);
		if ((temp_pos.SERVO_ROTATE_POS != 0 || temp_pos.SERVO_VERTICAL_POS != 0 || temp_pos.SERVO_EXTEND_POS != 0 || temp_pos.SERVO_GRIPPER_POS != 0 || temp_pos.SERVO_MOVE_SPEED != 0) &&
			(temp_pos.SERVO_ROTATE_POS != 65535 || temp_pos.SERVO_VERTICAL_POS != 65535 || temp_pos.SERVO_EXTEND_POS != 65535 || temp_pos.SERVO_GRIPPER_POS != 65535 || temp_pos.SERVO_MOVE_SPEED != 255))
		{
			if (temp_pos.SERVO_ROTATE_POS == 1 && temp_pos.SERVO_VERTICAL_POS == 2 && temp_pos.SERVO_EXTEND_POS == 3 && temp_pos.SERVO_GRIPPER_POS == 4 && temp_pos.SERVO_MOVE_SPEED == 5)
			{
				Serial.print("Address ");
				Serial.print(addr);
				Serial.println(": Program end");
			}
			else {
				Serial.print("Address ");
				Serial.print(addr);
				Serial.print(" used. ");
				print_info(temp_pos);
				address_for_save++;
			}
		}
		else
		{
			Serial.print("Address ");
			Serial.print(addr);
			Serial.println(" empty");
		}
	}
}
