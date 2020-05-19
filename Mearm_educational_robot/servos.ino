void setServoPulse(uint8_t n, double pulse) {
	double pulselength;

	pulselength = 1000000;   // 1,000,000 us per second
	#ifdef SERVOS_WORKING
		pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
	#endif	
	Serial.print(pulselength); Serial.println(" us per period");
	pulselength /= 4096;  // 12 bits of resolution
	Serial.print(pulselength); Serial.println(" us per bit");
	pulse *= 1000000;  // convert input seconds to us
	pulse /= pulselength;
	Serial.println(pulse);

	#ifdef SERVOS_WORKING
		pwm.setPWM(n, 0, pulse);
	#endif
}
