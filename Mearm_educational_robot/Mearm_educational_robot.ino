#include <jm_PCF8574.h>



#include "Arduino.h"
#include <Wire.h>
// #include <PCF8574.h>
#include <Adafruit_PWMServoDriver.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SdFat.h>

#define SERVOS_WORKING

#ifdef SERVOS_WORKING

	#define SERVOMIN  100 
	#define SERVOMAX  450
	#define USMIN  600
	#define USMAX  2400
	#define SERVO_FREQ 50

#endif // SERVOS_WORKING

#define CHIP_SELECT 4

#define ROTORY_ENCODER_PUSHBUTTON P2
#define ROTORY_ENCODER_CLK P0
#define ROTORY_ENCODER_DT P1

#define SERVO_GRIPPER 15
#define SERVO_EXTEND 13
#define SERVO_VERTICAL 14
#define SERVO_ROTATE 12

#define MAX_PROGRAM_STEPS 30

#define PCF0 0
#define PCF1 1

#define BUTTON_ROTATE_PLUS_PCF0 0
#define BUTTON_ROTATE_MINUS_PCF0 1

#define BUTTON_VERTICAL_DOWN_PCF0 2
#define BUTTON_VERTICAL_UP_PCF0 3

#define BUTTON_EXTEND_MINUS_PCF0 4
#define BUTTON_EXTEND_PLUS_PCF0 5

#define BUTTON_GRIPPER_OPEN_PCF0 6
#define BUTTON_GRIPPER_CLOSE_PCF0 7



#define BUTTON_SPEED_SELECT_PCF1 0
#define BUTTON_SAVE_POSITION_PCF1 1
#define BUTTON_RUN_PROGRAM_PCF1 2
#define BUTTON_ERASE_PROGRAM_PCF1 3
#define BUTTON_STEP_MODE_PCF1 4
#define BUTTON_STEP_BACK_PCF1 5
#define BUTTON_EXIT_PROGRAM_PCF1 6

#define BUTTON_DELAY 100

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

jm_PCF8574 pcf8574[2] = {0x20 , 0x21};

#ifdef SERVOS_WORKING
	Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#endif

rotoryEncoder rotory_encoder;
servo_limits limits;
servo_positions positions;

unsigned long now, last_save_time = 0, last_save_position = 0, last_erase = 0, last_speed_change_time;
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

uint8_t watchdog_counter = 0;

void setup() 
{
	Serial.begin(9600);
	Serial.println("MeArm Robot 2020");
	Serial.println("Initializing SD card...");	
//	pinMode(CHIP_SELECT, OUTPUT);
//	while (!SD.begin(CHIP_SELECT)) 
//	{
//		Serial.println("Card failed, or not present");
//		delay(500);
//	}
//	Serial.println("card initialized.");

	Wire.begin();
	#ifdef SERVOS_WORKING
		pwm.begin();
		pwm.setOscillatorFrequency(27000000);
		pwm.setPWMFreq(SERVO_FREQ);
		Serial.println("Servo driver initialised");
	#endif
	

	buttons_init();
	memory_read();

	get_coords(positions, 0);

	delay(10);
	Serial.print("Starting position: ");
	Serial.println(print_info(positions));
}


void loop() 
{	
	now = millis();
	mainloop();
	// test_pcf();
	test_servos(); // need to fully test this

	if (now - last_save_time > 1000)
	{
		last_save_time = now;
		Serial.println(watchdog_counter);
		watchdog_counter++;
	}
}
