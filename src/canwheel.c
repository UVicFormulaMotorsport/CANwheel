/*
  This file is part of the UVic Formula Motorsport CANwheel project.

  Copyright (c) 2017 Darryl Ring

  This program is free software: you can redistribute it and/or modify it under
  the terms of the GNU General Public License as published by the Free Software
  Foundation, either version 3 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "canwheel.h"

volatile uint16_t cw_adc_clock = 0;
volatile uint8_t cw_adc_trigger = 0;
volatile uint8_t cw_can_clock = 0;
volatile uint8_t cw_can_trigger = 0;
volatile uint8_t cw_input_clock = 0;
volatile uint8_t cw_input_trigger = 0;

volatile uint16_t rpm = 0xFFFF;
volatile uint8_t cal = 0;

void cw_timer_init(void);

int
main(void)
{
	/* Initialize peripherals */
	cw_adc_init();
	cw_io_init();
	cw_can_init();
	cw_timer_init();
	cw_leds_init();
	
	cw_leds_set_amber(1);
	cw_leds_set_green(1);
	cw_leds_update();
	
	_delay_ms(250);
	
	cw_leds_set_amber(0);
	cw_leds_set_green(0);
	cw_leds_set_shift(12);
	cw_leds_update();
	
	_delay_ms(250);
	
	cw_leds_set_shift(0);
	cw_leds_update();
	
	/* Enable global interrupts */
	sei();
	
	for (;;) {
		
		/* Perform periodic input tasks */
		if (cw_adc_trigger) {
			cw_adc_update();
			cw_adc_trigger = 0;
		}
		
		if (cw_input_trigger) {
			cw_io_update();
			cw_input_trigger = 0;
		}

		if (cal == 2) {
			cw_leds_set_amber(1);
			cw_leds_set_green(0);
		} else if (cal == 3) {
			cw_leds_set_amber(0);
			cw_leds_set_green(1);
		} else if (cal == 4) {
			cw_leds_set_amber(1);
			cw_leds_set_green(1);
		} else {
			cw_leds_set_amber(0);
			cw_leds_set_green(0);
		}
		
		/* Update shift LEDs */
		if (rpm == 0) {
			cw_leds_set_off(1);
		} else {
			cw_leds_set_off(0);
			
			if (rpm >= 12500) {
				cw_leds_set_blink(1);
			} else {
				cw_leds_set_blink(0);
				
				if      (rpm >= 12000) cw_leds_set_shift(12);
				else if (rpm >= 11000) cw_leds_set_shift(11);
				else if (rpm >= 10000) cw_leds_set_shift(10);
				else if (rpm >= 9000)  cw_leds_set_shift(9);
				else if (rpm >= 8000)  cw_leds_set_shift(8);
				else if (rpm >= 7000)  cw_leds_set_shift(7);
				else if (rpm >= 6000)  cw_leds_set_shift(6);
				else if (rpm >= 5000)  cw_leds_set_shift(5);
				else if (rpm >= 4000)  cw_leds_set_shift(4);
				else if (rpm >= 3000)  cw_leds_set_shift(3);
				else if (rpm >= 2000)  cw_leds_set_shift(2);
				else if (rpm >= 1000)  cw_leds_set_shift(1);
			}
		}
		
		cw_leds_update();
		
		/* Perform periodic output tasks */
		if (cw_can_trigger) {
			cw_can_send_inputs();
			cw_can_trigger = 0;
		}
	}
	
	return 0;
}

void
cw_rpm_set(uint16_t new_rpm)
{
	rpm = new_rpm;
}

void
cw_cal_set(uint8_t new_cal)
{
	cal = new_cal;
}

void
cw_timer_init(void)
{
	/* Output compare interrupts every 1 ms */
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A   = 999;

	/* Set timer clock prescaler to 8 for a 1 MHz timer clock */
	TCCR1B |= (1 << CS11);
}

ISR(TIMER1_COMPA_vect)
{
	cw_adc_clock++;
	cw_can_clock++;
	cw_input_clock++;

	/* Trigger ADC update */
	if (cw_adc_clock >= CW_ADC_INTERVAL) {
		cw_adc_clock = 0;
		cw_adc_trigger = 1;
	}

	/* Trigger CAN messages */
	if (cw_can_clock >= CW_CAN_INTERVAL) {
		cw_can_clock = 0;
		cw_can_trigger = 1;
	}

	/* Trigger input update */
	if (cw_input_clock >= CW_INPUT_INTERVAL) {
		cw_input_clock = 0;
		cw_input_trigger = 1;
	}
}

//set timer up into PDU
// 
// learn AVR
//
//1 pin pdu to power solenoids
// two pins to 
//if in life cal there is a way to turn button press into arbitraty outputs based on table, timer
//rerig one of the functions (antilag?) 
//