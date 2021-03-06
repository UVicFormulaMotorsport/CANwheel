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

#ifndef __CANWHEEL_H__
#define __CANWHEEL_H__

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "cw_adc.h"
#include "cw_can.h"
#include "cw_io.h"
#include "cw_leds.h"

/* Check inputs every 10 ms */
#define CW_INPUT_INTERVAL 10

void cw_rpm_set(uint16_t new_rpm);
void cw_cal_set(uint8_t new_cal);

#endif
