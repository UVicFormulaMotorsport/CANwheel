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

#ifndef __CW_ADC_H__
#define __CW_ADC_H__

#include <stdint.h>

#define CW_ADC_INTERVAL 250

void cw_adc_init(void);

void cw_adc_update(void);

uint8_t cw_adc_value(uint8_t input);

#endif
