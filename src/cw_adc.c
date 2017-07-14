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

#include <stdint.h>

#include <avr/io.h>

#include "canwheel.h"

#include "cw_adc.h"

volatile uint8_t cw_adc_value_1 = 0;
volatile uint8_t cw_adc_value_2 = 0;

void
cw_adc_init(void)
{
	/* Enable internal reference, left-align result */
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	
	/* Enable ADC */
	ADCSRA = (1 << ADEN);
	ADCSRB = (1 << AREFEN);
	
	/* Disable digital input buffers for analog inputs */
	DIDR0 = (1 << ADC5D); /* AIN 1 */
	DIDR1 = (1 << ADC8D); /* AIN 2 */
	
	/* Initialize ADC by performing a conversion */
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
}

void
cw_adc_update(void)
{
	volatile uint8_t ignore;
	
	/* Convert input 1 */
	ADMUX &= ~0x0F;
	ADMUX |= 5;
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	cw_adc_value_1 = ADCH;
	ignore = ADCL;
	
	/* Convert input 2 */
	ADMUX &= ~0x0F;
	ADMUX |= 8;
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	cw_adc_value_2 = ADCH;
	ignore = ADCL;
}

uint8_t
cw_adc_value(uint8_t input)
{
	if (input == 1) return cw_adc_value_1;
	else if (input == 2) return cw_adc_value_2;
	else return 0;
}
