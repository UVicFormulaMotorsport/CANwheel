#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "canwheel.h"

#include "cw_leds.h"

volatile uint8_t cw_leds_blinking = 0;
volatile uint8_t cw_leds_off = 0;

volatile uint8_t pwm[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void cw_leds_init(void)
{
	/* Configure TLC5940 interface pins as outputs */
	DDRB |= 0xC2; /* 1100_0010 (SCLK, BLANK, SIN) */
	DDRC |= 0x02; /* 0000_0010 (XLAT) */
	DDRD |= 0x44; /* 0100_0100 (DCSEL, GSCLK) */
	
	/* Enable SPI in master mode */
	SPCR = (1 << MSTR) | (1 << SPE);
	SPSR |= (1 << SPI2X);
}

void cw_leds_update(void)
{
	uint16_t i;
	
	PORTB |= (1 << PORTB6);

	// Write grayscale data
	for (i = 0; i < 24; i++) {
		SPDR = pwm[i];
		while (!(SPSR & (1 << SPIF)));
	}
	
	PORTB &= ~(1 << PORTB6);

	// Toggle XLAT
	PORTC |= (1 << PORTC1);
	PORTC &= ~(1 << PORTC1);
	
	// Toggle GSCLK 4096 times!
	for (i = 0; i < 256; i++) {
		PORTD |= (1 << PORTD2);
		PORTD &= ~(1 << PORTD2);
	}
}

void cw_leds_set_amber(uint8_t on)
{
	if (on) {
		pwm[1] |= 0x0f;
		pwm[2]  = 0xff;
	} else {
		pwm[1] &= 0xf0;
		pwm[2]  = 0x00;
	}
}

void cw_leds_set_green(uint8_t on)
{
	if (on) {
		pwm[21]  = 0xff;
		pwm[22] |= 0xf0;
	} else {
		pwm[21]  = 0x00;
		pwm[22] &= 0x0f;
	}
}

void cw_leds_set_shift(uint8_t count)
{
	uint8_t k;
	uint8_t d = 0;

	for (k = 1; k <= 12; k++) {
		if (count >= k) {
			if (d == 0) {
				pwm[((k + 1) >> 1) * 3] = 0xFF;
				pwm[((k + 1) >> 1) * 3 + 1] |= 0xF0;
				} else {
				pwm[2 + ((k) >> 1) * 3] = 0xFF;
				pwm[((k + 1) >> 1) * 3 + 1] |= 0x0F;
			}
		} else {
			if (d == 0) {
				pwm[((k + 1) >> 1) * 3] = 0x00;
				pwm[((k + 1) >> 1) * 3 + 1] &= 0x0F;
				} else {
				pwm[2+((k) >> 1) * 3] = 0x00;
				pwm[((k + 1) >> 1) * 3 + 1] &= 0xF0;
			}
		}

		if (d == 0) d = 1;
		else        d = 0;
	}
}

void
cw_leds_set_off(uint8_t off)
{
	if (!cw_leds_off && off) {
		cw_leds_set_shift(0);
		pwm[10] = 0x0F;
		pwm[11] = 0xFF;
		pwm[12] = 0xFF;
		pwm[13] = 0xF0;
		cw_leds_off = 1;
	} else if (cw_leds_off && !off) {
		cw_leds_set_shift(0);
		cw_leds_off = 0;
	}
}

void cw_leds_set_blink(uint8_t blink)
{
	cw_leds_blinking = blink;
}
