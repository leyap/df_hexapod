#include <Arduino.h>
#include "keycheck.h"

#define up_button	8
#define down_button	A1
#define left_button	9
#define right_button	12

#define stick_button A0
#define level_stick A2
#define vertical_stick A3

 uint16_t key_analog_range;
 uint16_t key_delay_time;
 
int dtime = 30;

int vertical_value=0;
int level_value=0;

//
void setup () {
	pinMode (stick_button, INPUT);
	pinMode (level_stick, INPUT);
	pinMode (vertical_stick, INPUT);

	pinMode (up_button, INPUT);
	pinMode (down_button, INPUT);
	pinMode (left_button, INPUT);
	pinMode (right_button, INPUT);
	Serial.begin (9600);
	key_delay_time = 1;	//
}

//
void loop () {

	vertical_value = analogRead (vertical_stick);
	level_value = analogRead (level_stick);

	if 	(level_value < 490 && (abs (level_value-510) >= abs (vertical_value-510)))
		Serial.write ('+');
	else if (level_value > 530 && (abs (level_value-510) > abs (vertical_value-510)))
		Serial.write ('-');
	else if (vertical_value < 490 && (abs (vertical_value-510) >= abs (level_value-510)))
		Serial.write  ('<');
	else if (vertical_value > 530 && (abs (vertical_value-510) > abs (level_value-510)))
		Serial.write ('>');
	else if (ispressed_in (up_button, 0))
		Serial.write ('H');
	else if (ispressed_in (down_button, 0))
		Serial.write ('L');
	else if (ispressed_in (left_button, 0))
		Serial.write ('h');
	else if (ispressed_in (right_button, 0))
		Serial.write ('l');
	delay (dtime);
}




//digital pin key 0 or 1
boolean ispressed_in (uint16_t key, uint16_t state) {
	if (digitalRead (key) == state) {
		delay (key_delay_time);
		if (digitalRead (key) == state)
			return true;
	}
	return false;
}


//analog pin key 
boolean issticked_in (uint16_t key, uint16_t range) {
	if (abs (analogRead (key) - range) < key_analog_range) {
		delay (key_delay_time);
		if (abs (analogRead (key) - range) < key_analog_range)
			return true;
	}
	return false;
}

//analog pin key
boolean issticked2_in (uint16_t key, uint16_t left, uint16_t right) {
	uint16_t value = analogRead (key);
	if (value >= left && value < right) {
		delay (key_delay_time);
		value = analogRead (key);
		if (value >= left && value < right) 
			return true;
	}
	return false;
}


