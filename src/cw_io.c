#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "canwheel.h"

#include "cw_io.h"

uint8_t cw_io_values = 0;
uint8_t flag2 = 0;
uint8_t flag7 = 0;

void
cw_io_init(void)
{
	/* Enable internal pull-ups for input pins */
	PORTB |= 0x38; /* 0011_1000 (6, 4, 3) */
	PORTC |= 0xE1; /* 1110_0001 (5, 2, 1, 8) */
	PORTD |= 0x01; /* 0000_0001 (7) */
}

void
cw_io_update(void)
{
	if (PINC & (1 << PINC5)) cw_io_values &= ~0x01;
	else cw_io_values |= 0x01;
	
	if (PINC & (1 << PINC6)){
		cw_io_values |= 0x02;
		flag2 = 1;
	}
	else{ 
		cw_io_values &= ~0x02;
		//flag2 = 0;
	}
	if (PINB & (1 << PINB3)) cw_io_values |= 0x04;
	else cw_io_values &= ~0x04;
	
	if (PINB & (1 << PINB4)) cw_io_values |= 0x08;
	else cw_io_values &= ~0x08;
	
	if (PINC & (1 << PINC7)) cw_io_values |= 0x10;
	else cw_io_values &= ~0x10;
	
	if (PINB & (1 << PINB5)) cw_io_values |= 0x20;
	else cw_io_values &= ~0x20;
	
	if (PIND & (1 << PIND0)){
		cw_io_values |= 0x40;
		flag7 = 1;
	}
	else {
		cw_io_values &= ~0x40;
		//flag7 = 0;
	}
	if (PINC & (1 << PINC0)) cw_io_values &= ~0x80;
	else cw_io_values |= 0x80;
}

uint8_t
cw_io_value(uint8_t input)
{
	return cw_io_values & (1 << (input - 1));
}

uint8_t
cw_io_flags(uint8_t flag)
{
	if (flag == 2){
		return flag2;
	}
	if (flag ==7){
		return flag7;
	}
}