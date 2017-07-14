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

#ifndef __CW_CAN_H__
#define __CW_CAN_H__

/* CAN input status message ID */
#define CW_CAN_ID 0x500

/* CAN transmit and receive pages */
#define CW_CAN_PAGE_RX 0
#define CW_CAN_PAGE_TX 1

/*
  CAN message interval (in milliseconds)

  The input status messages will be sent after every interval using the counter
  from Timer/Counter1.

  Default is 100 ms for a 10 Hz update rate
*/
#define CW_CAN_INTERVAL 100

void cw_can_init(void);

void cw_can_send(uint8_t page, uint16_t id, volatile uint8_t msg[], uint8_t len);

void cw_can_send_inputs(void);

#endif
