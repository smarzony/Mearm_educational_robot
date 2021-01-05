void auto_mode()
{
	String output_text;
	output_text = "Program start";
	Serial.println(output_text);

//	file_append("dupa.txt", "witam");
	servo_positions program_step;
	uint16_t program_step_no = 0;
	exit_program = false;
	while (1)
	{
		read_button_neg_switch_pcf2(PCF1, BUTTON_EXIT_PROGRAM_PCF1, exit_program, BUTTON_DELAY);
		if (exit_program)
		{
			break;
		}

		if (pcf8574[PCF1].digitalRead(BUTTON_STEP_MODE_PCF1) == false) // this is faster
		{
			step_mode = true;
			Serial.println("Step mode ON");
			delay(500);
		}

		get_coords(program_step, program_step_no + 1);
		if ((program_step.SERVO_ROTATE_POS == 1 && program_step.SERVO_VERTICAL_POS == 2 && program_step.SERVO_EXTEND_POS == 3 && program_step.SERVO_GRIPPER_POS == 4 && program_step.SERVO_MOVE_SPEED == 5) ||
			(program_step_no > MAX_PROGRAM_STEPS)
			)
		{
			get_coords(positions, program_step_no - 1);
			break;
		}
		else if (program_step.SERVO_ROTATE_POS == 0 && program_step.SERVO_VERTICAL_POS == 0 && program_step.SERVO_EXTEND_POS == 0 && program_step.SERVO_GRIPPER_POS == 0 && program_step.SERVO_MOVE_SPEED == 0)
		{
			Serial.println("No program!");
			break;
		}
		else
		{
			output_text = "Step ";
			output_text += program_step_no;
			output_text += ". ";
			output_text += print_info(program_step);
			Serial.println(output_text);

			/*
			Serial.print("Step ");
			Serial.print(program_step_no);
			Serial.print(". ");
			Serial.println(print_info(program_step));
			*/
			set_position(program_step);
			program_step_no++;
			if (step_mode == false)
			{
				switch (program_step.SERVO_MOVE_SPEED)
				{
				case 0:
					delay(2000);
					break;

				case 1:
					delay(1500);
					break;

				case 2:
					delay(1200);
					break;

				case 3:
					delay(1000);
					break;
				}
			}
			else
			{
				make_step = false;
				step_back = false;
				saving = false;
				erasing = false;
				//         exit_program = false;
				positions = program_step;
				save_coords(positions, 0);
				while (make_step == false && step_back == false && step_mode == true && exit_program == false)
				{
					read_button_neg_switch_pcf2(PCF1, BUTTON_RUN_PROGRAM_PCF1, make_step, BUTTON_DELAY);
					read_button_neg_switch_pcf2(PCF1, BUTTON_STEP_BACK_PCF1, step_back, BUTTON_DELAY);
					read_button_neg_switch_pcf2(PCF1, BUTTON_SAVE_POSITION_PCF1, saving, BUTTON_DELAY);
					read_button_neg_switch_pcf2(PCF1, BUTTON_ERASE_PROGRAM_PCF1, erasing, BUTTON_DELAY);
					read_button_neg_switch_pcf2(PCF1, BUTTON_EXIT_PROGRAM_PCF1, exit_program, BUTTON_DELAY);
					step_mode_switch_pcf(PCF1, BUTTON_STEP_MODE_PCF1);
					manual_mode();
					if (saving == true)
					{
						Serial.print("Modyfying step: ");
						Serial.println(program_step_no - 1);
						save_coords(positions, program_step_no);
						saving = false;
					}
					if (erasing == true)
					{
						servo_positions program_end;
						program_end.SERVO_ROTATE_POS = 1;
						program_end.SERVO_VERTICAL_POS = 2;
						program_end.SERVO_EXTEND_POS = 3;
						program_end.SERVO_GRIPPER_POS = 4;
						program_end.SERVO_MOVE_SPEED = 5;
						save_coords(program_end, program_step_no + 1);
						erase_program(program_step_no + 2);
						Serial.print("Program ended on step ");
						Serial.println(program_step_no - 1);
						address_for_save = program_step_no + 1;
						erasing = false;
					}
				}
				if (step_back == true)
				{
					if (program_step_no > 1)
					{
						program_step_no -= 2;
						Serial.print("Step back to ");
						Serial.println(program_step_no);
					}
					else
					{
						Serial.println("Can't go back");
						program_step_no = 0;
					}
				}
				if (exit_program == true)
					break;
			}
		}
	}
	if (exit_program == true)
		Serial.println("Program exited");
	Serial.println("Program end");
	get_coords(positions, program_step_no);
	Serial.println(print_info(positions));
	program_run = false;
	step_mode = false;
}
