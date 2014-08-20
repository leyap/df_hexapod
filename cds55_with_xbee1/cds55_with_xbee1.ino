
#include <Arduino.h>
#include <SPI.h>
//#include <ServoCds55.h>
//#include "pins_arduino.h"

#include "split.h"
#include "read_serial.h"

#include "cds55.h"
#include "hexapod.h"

#define BUF_SIZE 3

char buf[BUF_SIZE];

float l2_leg1_angle;
float l2_leg2_value;
float l1_leg1_angle;
float l1_leg2_value;

float r1_leg1_angle;
float r1_leg2_value;
float r2_leg1_angle;
float r2_leg2_value;

float left_step = 0;
float right_step = 0;
float step_one = 18*3.14/180;
uint8_t r = 10;
uint8_t r2 = 30;
int16_t leg2 = 50;
int16_t leg3 = 30;

int16_t l2_leg2_angle;
int16_t l1_leg2_angle;

int16_t r1_leg2_angle;
int16_t r2_leg2_angle;

int old_cmd = '+';


//
void setup () {
	Serial.begin (9600);
	SPI.begin ();
	//SPI.setClockDivider (SPI_CLOCK_DIV8);
	delay (2000);
	digitalWrite (SS, LOW);
	ctrl_all_leg (0, 0, 0);
	digitalWrite (SS, HIGH);
}

//
void loop () {
	digitalWrite (SS, LOW);
	if (read_serial ( buf, BUF_SIZE)) {
		if (buf[0] != old_cmd) {
			left_step = right_step;
		}

		old_cmd = buf[0];
		if (buf[0] =='+') {
			left_step -= step_one;
			right_step -= step_one;
		} else if (buf[0] == '-') {
			left_step += step_one;
			right_step += step_one;
		} else if (buf[0] == '<') {
			left_step -= step_one;
			right_step += step_one;
		} else if (buf[0] == '>') {
			left_step += step_one;
			right_step -= step_one;
		} 
		if (buf[0] == 'H') {
			leg2 += 3;
			if (leg2 > 300) leg2 = 300;
		} else if (buf[0] == 'L') {
			leg2 -= 3;
			if (leg2 < 0) leg2 = 0;
		} 
		if (buf[0] == 'h') {
			leg3 += 3;
			if (leg3 > 300) leg3 = 300;
		} else if (buf[0] == 'l') {
			leg3 -= 3;
			if (leg3 < 0) leg3 = 0;
		}


		l2_leg1_angle = r * sin (left_step);
		l2_leg2_value = r2 * cos (left_step);
		l1_leg1_angle = -r * sin (left_step);
		l1_leg2_value = -r2 * cos (left_step);

		r1_leg1_angle = r * sin (right_step);
		r1_leg2_value = r2 * cos (right_step);
		r2_leg1_angle = -r * sin (right_step);
		r2_leg2_value = -r2 * cos (right_step);

		if (l2_leg2_value > 0) l2_leg2_value = 0;
		if (l1_leg2_value > 0) l1_leg2_value = 0;

		if (r1_leg2_value > 0) r1_leg2_value = 0;
		if (r2_leg2_value > 0) r2_leg2_value = 0;

		l2_leg2_angle = l2_leg2_value+leg2;
		l1_leg2_angle = l1_leg2_value +leg2;
		l2_leg2_angle = constrain (l2_leg2_angle, 0, 250);
		l1_leg2_angle = constrain (l1_leg2_angle, 0, 250);

		r1_leg2_angle = r1_leg2_value+leg2;
		r2_leg2_angle = r2_leg2_value+leg2;
		r1_leg2_angle = constrain (r1_leg2_angle, 0, 250);
		r2_leg2_angle = constrain (r2_leg2_angle, 0, 250);

		left_two (l2_leg1_angle, l2_leg2_angle, leg3, 1);
		right_one (r1_leg1_angle, r1_leg2_angle, leg3, 1);

		right_two (r2_leg1_angle, r2_leg2_angle, leg3, 1);
		left_one (l1_leg1_angle, l1_leg2_angle, leg3, 1);
		//step += step_one;
		delay (10);
	}
	digitalWrite (SS, HIGH);
}


