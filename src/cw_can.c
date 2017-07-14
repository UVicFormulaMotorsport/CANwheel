#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "canwheel.h"

#include "cw_can.h"

void cw_can_set_id(uint16_t id);
void cw_can_set_mask(uint16_t mask);

void
cw_can_init(void)
{
	/* Configure CAN_TX pin as output */
	DDRC |= (1 << DDC2);

	/* Reset the CAN controller */
	CANGCON = (1 << SWRES);

	/*
	  Configure CAN bit timing
	  Baud rate of 1 Mbps with a 16 MHz clock. See Table 19-2 on page 188 of the
	  data sheet.
	*/
	CANBT1 = 0x02;
	CANBT2 = 0x04;
	CANBT3 = 0x13;

	/* Enable interrupts */
	CANGIE |= (1 << ENIT) | (1 << ENRX);
	CANIE2 |= (1 << IEMOB0);

	/* Configure message receive on page 0 */
	CANPAGE  = 0;
	CANCDMOB = 0x00;
	CANSTMOB = 0x00;

	cw_can_set_id(0x0000);
	cw_can_set_mask(0x0000);

	/* All input messages are 8 bytes */
	CANCDMOB |= 8;

	/* Enable receive */
	CANCDMOB |= (1 << CONMOB1);

	/* Enable CAN controller */
	CANGCON |= (1 << ENASTB);
}

void
cw_can_send(uint8_t page, uint16_t id, volatile uint8_t msg[], uint8_t len)
{
	uint8_t i;
	cli();
	
	CANPAGE = page << MOBNB0;

	CANCDMOB = 0x00;
	CANSTMOB = 0x00;

	cw_can_set_id(id);

	for (i = 0; i < len; i++) {
		CANMSG = msg[i];
	}
	
	CANCDMOB |= len;
	CANCDMOB |= (1 << CONMOB0);

	while((CANSTMOB & (1 << TXOK)) != (1 << TXOK));
	CANCDMOB = 0x00;
	CANSTMOB = 0x00;
	
	sei();
}

void
cw_can_send_inputs(void)
{
	uint8_t msg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
		
	msg[1] = (PINC & (1 << PINC6)) > 0;
	msg[3] = (PINC & (1 << PINC0)) > 0;
	cw_can_send(1, 0x400, msg, 8);
	
		/*
	msg[1] = cw_io_value(1);
	msg[3] = cw_io_value(2);
	msg[5] = cw_io_value(3);
	msg[7] = cw_io_value(4);
	cw_can_send(1, 0x400, msg, 8);
	msg[1] = cw_io_value(5);
	msg[3] = cw_io_value(6);
	msg[5] = cw_io_value(7);
	msg[7] = cw_io_value(8);
	cw_can_send(1, 0x401, msg, 8);
	msg[1] = cw_adc_value(1);
	msg[3] = cw_adc_value(2);
	cw_can_send(1, 0x402, msg, 8);
	*/
}

void
cw_can_set_id(uint16_t id)
{
	CANIDT4 = 0;
	CANIDT3 = 0;
	CANIDT2 = (uint8_t)  id << 5;
	CANIDT1 = (uint16_t) id >> 3;
}

uint16_t
cw_can_get_id(void)
{
	uint16_t id = 0;
	
	id |= (CANIDT2 >> 5);
	id |= (CANIDT1 << 3);

	return id;
}

void
cw_can_set_mask(uint16_t mask)
{
	CANIDM4 = 0;
	CANIDM3 = 0;
	CANIDM2 = (uint8_t)  mask << 5;
	CANIDM1 = (uint16_t) mask >> 3;
}

ISR(CAN_INT_vect)
{
	uint16_t id;
	uint8_t  len;
	uint8_t  idx;
	uint8_t  byte;
	uint16_t rpm = 0;
	uint8_t cal = 0;

	CANPAGE = 0;

	id  = cw_can_get_id();
	len = (CANCDMOB & 0x0F);
	
	if (id == 0x600) {
		rpm = 0;
		
		// RPM signal
		for (idx = 0; idx < len; idx++) {
			byte = CANMSG;
			
			if (idx == 0) {
				rpm = (uint16_t) byte << 8;
			} else if (idx == 0) {
				rpm |= byte;
			}
		}
		
		cw_rpm_set(rpm);
		
	} else if (id == 0x60A) {
		// Cal signal
		for (idx = 0; idx < len; idx++) {
			byte = CANMSG;
			
			if (idx == 3) {
				cal = byte;
			}
		}
		
		cw_cal_set(cal);
	}
	
	/* Re-enable transmission */
	CANSTMOB = 0;
	CANCDMOB |= (1 << CONMOB1);
}
