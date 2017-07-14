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

#ifndef __CW_LEDS_H__
#define __CW_LEDS_H__

void cw_leds_init(void);

void cw_leds_update(void);

void cw_leds_set_amber(uint8_t on);
void cw_leds_set_green(uint8_t on);

void cw_leds_set_shift(uint8_t count);
void cw_leds_set_off(uint8_t off);
void cw_leds_set_blink(uint8_t blink);

#endif
