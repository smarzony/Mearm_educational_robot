/***************************************************
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These drivers use I2C to communicate, 2 pins are required to
  interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <PCF8574.h>
#include <Adafruit_PWMServoDriver.h>
#include <EEPROM.h>

//#define SERVOS_WORKING

#ifdef SERVOS_WORKING

	#define SERVOMIN  100 
	#define SERVOMAX  450
	#define USMIN  600
	#define USMAX  2400
	#define SERVO_FREQ 50

#endif // SERVOS_WORKING



#define ROTORY_ENCODER_PUSHBUTTON P2
#define ROTORY_ENCODER_CLK P0
#define ROTORY_ENCODER_DT P1

#define SERVO_GRIPPER 15
#define SERVO_EXTEND 13
#define SERVO_VERTICAL 14
#define SERVO_ROTATE 12

#define MAX_PROGRAM_STEPS 30

#define BUTTON_ROTATE_PLUS_PCF0 0
#define BUTTON_ROTATE_MINUS_PCF0 1

#define BUTTON_VERTICAL_DOWN_PCF0 2
#define BUTTON_VERTICAL_UP_MAIN 6

#define BUTTON_EXTEND_MINUS_PCF0 4
#define BUTTON_EXTEND_PLUS_PCF0 5

#define BUTTON_GRIPPER_OPEN_PCF0 6
#define BUTTON_GRIPPER_CLOSE_PCF0 7


#define BUTTON_SPEED_SELECT_MAIN 5

#define BUTTON_SAVE_POSITION_MAIN 4
#define BUTTON_RUN_PROGRAM_MAIN 3
#define BUTTON_ERASE_PROGRAM_MAIN 2
#define BUTTON_STEP_MODE_MAIN A2
#define BUTTON_STEP_BACK_MAIN A1
#define BUTTON_EXIT_PROGRAM_MAIN A3

#define BUTTON_DELAY 400

struct servo_limits
{
	uint16_t SERVO_ROTATE_MIN = 100;
	uint16_t SERVO_ROTATE_MAX = 400;

	uint16_t SERVO_VERTICAL_MIN = 190;
	uint16_t SERVO_VERTICAL_MAX = 400;

	uint16_t SERVO_EXTEND_MIN = 200;
	uint16_t SERVO_EXTEND_MAX = 380;

	uint16_t SERVO_GRIPPER_MIN = 200;
	uint16_t SERVO_GRIPPER_MAX = 350;
};

struct servo_positions
{
	uint16_t SERVO_ROTATE_POS = 0;
	uint16_t SERVO_VERTICAL_POS = 0;
	uint16_t SERVO_EXTEND_POS = 0;
	uint16_t SERVO_GRIPPER_POS = 0;
	uint8_t SERVO_MOVE_SPEED = 0;
};

struct rotoryEncoder {
	bool clk_actual,
		clk_prev,
		dt_actual,
		dt_prev;
	int switch_state;
	int value;
};

PCF8574 pcf8574[2] = { 0x20, 0x21 };


#ifdef SERVOS_WORKING
	Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#endif



rotoryEncoder rotory_encoder;
servo_limits limits;
servo_positions positions;

unsigned long now, last_save_time = 0, last_save_position = 0, last_erase = 0;
uint16_t address_for_save = 0;
bool servo_pos_changed[4];
bool program_run = false;
bool saving = false;
bool erasing = false;
bool step_mode = false;
bool make_step = false;
bool step_back = false;
bool read_memory = false;
bool exit_program = false;

void setup() {
	Serial.begin(9600);
	Serial.println("MeArm Robot 2020");

	#ifdef SERVOS_WORKING
		pwm.begin();
		pwm.setOscillatorFrequency(27000000);
		pwm.setPWMFreq(SERVO_FREQ);
	#endif
	

	
	buttons_init();
	memory_read();

	get_coords(positions, 0);

	#ifdef SERVOS_WORKING
		pwm.setPWM(SERVO_ROTATE, 0, positions.SERVO_ROTATE_POS);
		pwm.setPWM(SERVO_VERTICAL, 0, positions.SERVO_VERTICAL_POS);
		pwm.setPWM(SERVO_EXTEND, 0, positions.SERVO_EXTEND_POS);
		pwm.setPWM(SERVO_GRIPPER, 0, positions.SERVO_GRIPPER_POS);
	#endif

	delay(10);
	Serial.print("Starting position: ");
	print_info(positions);
}


void loop() {
	now = millis();
	if (now - last_save_time > 1000)
	{
		last_save_time = now;
		save_coords(positions, 0);
	}
	read_buttons();

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
}