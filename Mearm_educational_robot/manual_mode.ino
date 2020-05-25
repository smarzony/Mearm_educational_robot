
void manual_mode()
{
	int old_speed = positions.SERVO_MOVE_SPEED;
	read_button_inc_switch_pcf2(PCF1, BUTTON_SPEED_SELECT_PCF1, 0, 3, positions.SERVO_MOVE_SPEED, BUTTON_DELAY);
	if (old_speed != positions.SERVO_MOVE_SPEED)
	{
		Serial.print("Move speed: ");
		Serial.println(positions.SERVO_MOVE_SPEED);
	}
	//servo_pos_changed[0] = twoButtonControl_pcf(positions.SERVO_ROTATE_POS, 0, 1, positions.SERVO_MOVE_SPEED);
	
	servo_pos_changed[0] = twoButtonControl_pcf(positions.SERVO_ROTATE_POS, BUTTON_ROTATE_MINUS_PCF0, BUTTON_ROTATE_PLUS_PCF0, positions.SERVO_MOVE_SPEED, PCF0);
	if (positions.SERVO_ROTATE_POS < limits.SERVO_ROTATE_MIN)
		positions.SERVO_ROTATE_POS = limits.SERVO_ROTATE_MIN;
	else if (positions.SERVO_ROTATE_POS > limits.SERVO_ROTATE_MAX)
		positions.SERVO_ROTATE_POS = limits.SERVO_ROTATE_MAX;
	if (servo_pos_changed[0])
	{
		#ifdef SERVOS_WORKING
			pwm.setPWM(SERVO_ROTATE, 0, positions.SERVO_ROTATE_POS);
		#endif
	}
		

	servo_pos_changed[1] = twoButtonControl_pcf(positions.SERVO_VERTICAL_POS, BUTTON_VERTICAL_DOWN_PCF0, BUTTON_VERTICAL_UP_PCF0, positions.SERVO_MOVE_SPEED, PCF0);
	if (positions.SERVO_VERTICAL_POS < limits.SERVO_VERTICAL_MIN)
		positions.SERVO_VERTICAL_POS = limits.SERVO_VERTICAL_MIN;
	else if (positions.SERVO_VERTICAL_POS > limits.SERVO_VERTICAL_MAX)
		positions.SERVO_VERTICAL_POS = limits.SERVO_VERTICAL_MAX;
	if (servo_pos_changed[1])
	{
		#ifdef SERVOS_WORKING
			pwm.setPWM(SERVO_VERTICAL, 0, positions.SERVO_VERTICAL_POS);
		#endif
	}


	servo_pos_changed[2] = twoButtonControl_pcf(positions.SERVO_EXTEND_POS, BUTTON_EXTEND_PLUS_PCF0, BUTTON_EXTEND_MINUS_PCF0, positions.SERVO_MOVE_SPEED, PCF0);
	if (positions.SERVO_EXTEND_POS < limits.SERVO_EXTEND_MIN)
		positions.SERVO_EXTEND_POS = limits.SERVO_EXTEND_MIN;
	else if (positions.SERVO_EXTEND_POS > limits.SERVO_EXTEND_MAX)
		positions.SERVO_EXTEND_POS = limits.SERVO_EXTEND_MAX;
	if (servo_pos_changed[2])
	{
		#ifdef SERVOS_WORKING
			pwm.setPWM(SERVO_EXTEND, 0, positions.SERVO_EXTEND_POS);
		#endif
	}



	servo_pos_changed[3] = twoButtonControl_pcf(positions.SERVO_GRIPPER_POS, BUTTON_GRIPPER_OPEN_PCF0, BUTTON_GRIPPER_CLOSE_PCF0, positions.SERVO_MOVE_SPEED, PCF0);
	if (positions.SERVO_GRIPPER_POS < limits.SERVO_GRIPPER_MIN)
		positions.SERVO_GRIPPER_POS = limits.SERVO_GRIPPER_MIN;
	if (positions.SERVO_GRIPPER_POS > limits.SERVO_GRIPPER_MAX)
		positions.SERVO_GRIPPER_POS = limits.SERVO_GRIPPER_MAX;
	if (servo_pos_changed[3])
	{
		#ifdef SERVOS_WORKING
			pwm.setPWM(SERVO_GRIPPER, 0, positions.SERVO_GRIPPER_POS);
		#endif
	}

	if (servo_pos_changed[0] || servo_pos_changed[1] || servo_pos_changed[2] || servo_pos_changed[3])
	{
		print_info(positions);
	}
}

