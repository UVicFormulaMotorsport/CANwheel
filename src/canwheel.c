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
volatile uint8_t cw_lightshow_clock = 0;
volatile uint8_t cw_input_trigger = 0;

volatile uint16_t rpm = 0xFFFF;
volatile uint8_t cal = 0;
volatile uint8_t flag = 0;
volatile uint8_t i = 0;
volatile uint16_t random = 0xFFFF;
volatile uint16_t rpmVal[243] = {9929,9562,9342,9109,9073,8902,7678,4394,5066,5661,6118,6684,7038,7272,7549,7709,7654,7105,8283,7692,8223,8016,7896,7402,7255,6811,7123,7331,7890,8341,8239,7516,7193,6699,6653,6460,6904,7592,7269,8099,8475,8722,9216,9703,9841,10208,10700,10766,9959,9097,8462,7899,7141,6573,6531,6370,6645,6629,6821,6532,6260,6315,5944,6295,6585,6680,6906,7027,7569,8046,8068,7639,7004,6903,6597,7392,7068,6929,7777,8028,8342,8884,8929,7992,7687,7308,6730,6696,6384,6208,6114,7243,7887,9222,8842,9663,8482,8999,9548,9525,8585,8010,7057,7575,8822,8600,8848,9352,9044,7917,7272,7498,8649,8469,8587,8658,8560,8647,9341,9797,10073,10422,10839,11233,11781,12021,12454,12632,11851,11027,10712,9839,8345,6933,6496,6007,5252,4890,4761,6845,7118,5944,5892,6089,6254,5379,5130,4820,4625,4477,4858,5524,6365,5829,6160,6571,7107,7701,8204,8188,7815,7376,6741,6379,6015,5880,6369,5688,5832,5452,6241,6613,6574,6790,7375,7925,8249,8431,8659,9530,9230,9473,9715,9607,10058,10232,10297,9775,9458,9137,9806,10603,10109,9879,10103,10609,10759,10788,11168,10418,10068,9223,8690,7956,7304,6735,6756,6261,6478,6469,7199,7672,7970,8231,8225,7733,7018,8036,7602,8607,9195,9525,9649,9455,9876,9887,9831,9521,10906,11013,10712,11335,11819,12377,12313,12601,12985,13071,12916,12985,13024,12974,13024}; //1KB



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
	//if button is pressed and flag  = 0
	
	while(1){
		//rpm = rpmVal[i];
		if (cw_input_trigger) {
			cw_io_update();
			cw_input_trigger = 0;
		}		

			/* Update shift LEDs */
		if (rpm == 0) {
			cw_leds_set_shift(0);
			cw_leds_update();
			} 
		else {
			cw_leds_set_off(0);
		
			if (rpm >= 12500) {
				cw_leds_set_blink(1);
			} 

			else {
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

			
	if(rpm >= 12000){
		rpm = 0;
	}

	}
		
	
	//return 0;
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
	cw_lightshow_clock++;

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
	
	if(cw_lightshow_clock > 5){
		if(cw_io_flags(2) == 1){
			if(++i == 243) {
				i = 0; 
				rpm = 0;
			}
			rpm = rpmVal[i]; 
		}
		else{
			i = 0;
			rpm =0;
		}
		
		cw_lightshow_clock = 0;
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