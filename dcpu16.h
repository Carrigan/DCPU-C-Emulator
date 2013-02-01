/**
  	@author Brian Carrigan
  	@date 1/31/2013
	@file dcpu16.h
  	Email brian.c.carrigan@gmail.com
 
	This file is part of the DCPU C Emulator LPC-Library.

    The DCPU C Emulator LPC-Library is free software: you 
    can redistribute it and/or modify it under the terms of the GNU 
    General Public License as published by the Free Software Foundation, 
    either version 3 of the License, or (at your option) any later 
    version.

    The DCPU C Emulator LPC-Library is distributed in 
    the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
    even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
    PARTICULAR PURPOSE.  See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the DCPU C Emulator LPC-Library.  If not, 
    see <http://www.gnu.org/licenses/>.
 */


#ifndef DCPU_H
#define DCPU_H

void DCPU_Init(void);
void DCPU_Run(void);
void DCPU_Cycle(void);

#endif