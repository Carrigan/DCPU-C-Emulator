/**
  	@author Brian Carrigan
  	@date 1/31/2013
	@file dcpu16.c
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
 
 #include "dcpu16.h"

// Branch/Add test
//unsigned short DCPU_Memory[256] = {0x7DE1, 0x0003, 0x81C1, 0x0000};

// Loop Test
//unsigned short DCPU_Memory[256] = {0xFC01, 0x8081, 0xA48E, 0x99C1, 0x8482, 0x89C1, 0x81C1};

// Notch's Example 196608Kb memory needed.
unsigned short DCPU_Memory[65536] = {	0x7c01, 0x0030, 0x7de1, 0x1000, 0x0020, 0x7803, 0x1000, 0xc00d, 
										0x7dc1, 0x001a, 0xa861, 0x7c01, 0x2000, 0x2161, 0x2000, 0x8463,
										0x806d, 0x7dc1, 0x000d, 0x9031, 0x7c10, 0x0018, 0x7dc1, 0x001a,
										0x9037, 0x61c1, 0x7dc1, 0x001a, 0x0000, 0x0000, 0x0000, 0x0000 };

										unsigned short A, B, C, X, Y, Z, I, J, O, PC, SP;
unsigned short *REGISTER_LOCATION[8] = {&A, &B, &C, &X, &Y, &Z, &I, &J};
unsigned char waitState;

// Initialize the DCPU-16
void DCPU_Init(void)
{
	A = 0;
	B = 0;
	C = 0;
	X = 0;
	Y = 0;
	Z = 0;
	I = 0;
	J = 0;
	O = 0;
	PC = 0;
	SP = 0;

	waitState = 0;
}

/**
 *	@brief Goes through cycles.
 *
 */
void DCPU_Run(void)
{
	while(1)
	{
		// Wait for 100kHz timer
		DCPU_Cycle();
	}
} // Function

/**
 *	@brief Emulates a single cycle of the DCPU-16. 
 *	
 *	This is the meat of the project. If you want it to emulate the DCPU as laid out
 *	by Notch, run this at function at 100kHz.
 */
void DCPU_Cycle(void)
{
	// If there are no wait states from multi cycle instructions, execute next code.
	if(waitState == 0)
	{
		// Temporary Variables
		unsigned short *a, *b, a_n, b_n, a_val, b_val;
		unsigned short currentInstruction;
		unsigned short opCode;	

		// Fetch Instruction, Increment PC
		currentInstruction = DCPU_Memory[PC];
		PC++;

		// Get A/B/OP Code
		a_n = (currentInstruction & 0x03F0) >> 4;
		b_n = (currentInstruction & 0xFC00) >> 10;
		opCode = currentInstruction & 0xF;

		// Find the address of A
		if(a_n < 0x08)
		{
			a = REGISTER_LOCATION[a_n];
		} else if (a_n < 0x10) {
			a = &DCPU_Memory[*REGISTER_LOCATION[a_n - 0x08]];
		} else if (a_n < 0x18) {
			a = &DCPU_Memory[*REGISTER_LOCATION[a_n - 0x10] + DCPU_Memory[PC]];
			PC++;
			waitState++;
		} else if (a_n == 0x18) {
			a = &DCPU_Memory[SP];
			SP++;
		} else if (a_n == 0x19) {
			a = &DCPU_Memory[SP];
		} else if (a_n == 0x1A) {
			SP--;
			a = &DCPU_Memory[SP];
		} else if (a_n == 0x1B) {
			a = &SP;
		} else if (a_n == 0x1C) {
			a = &PC;
		} else if (a_n == 0x1D) {
			a = &O;
		} else if (a_n == 0x1E) {
			a = &DCPU_Memory[DCPU_Memory[PC]];
			PC++;
			waitState++;
		} else if (a_n == 0x1F) {
			a = &DCPU_Memory[PC];
			PC++;
			waitState++;
		} else {
			a_val = a_n - 0x20;
			a = &a_val;
		}


		// Find the address of B
		if(b_n < 0x08)
		{
			b = REGISTER_LOCATION[b_n];
		} else if (b_n < 0x10) {
			b = &DCPU_Memory[*REGISTER_LOCATION[b_n - 0x08]];
		} else if (b_n < 0x18) {
			b = &DCPU_Memory[*REGISTER_LOCATION[b_n - 0x10] + DCPU_Memory[PC]];
			PC++;
			waitState++;
		} else if (b_n == 0x18) {
			b = &DCPU_Memory[SP];
			SP++;
		} else if (b_n == 0x19) {
			b = &DCPU_Memory[SP];
		} else if (b_n == 0x1A) {
			SP--;
			b = &DCPU_Memory[SP];
		} else if (b_n == 0x1B) {
			b = &SP;
		} else if (b_n == 0x1C) {
			b = &PC;
		} else if (b_n == 0x1D) {
			b = &O;
		} else if (b_n == 0x1E) {
			b = &DCPU_Memory[DCPU_Memory[PC]];
			PC++;
			waitState++;
		} else if (b_n == 0x1F) {
			b = &DCPU_Memory[PC];
			PC++;
			waitState++;
		} else {
			b_val = b_n - 0x20;
			b = &b_val;
		}

		// Carry out instruction
		switch(opCode)
		{
			// Special! 
			case 0x00:
				switch(a_n)
				{
					// JSR
					case 0x01:
						SP--;
						DCPU_Memory[SP] = PC;
						PC = *b;
						waitState++;
						break;
				}
				break;
			// Set - 1 Cycle
			case 0x01:
				*a = *b;
				break;
			// Add - 2 Cycle
			case 0x02:
				unsigned int addition;
				waitState++;
				addition = *a + *b;
				*a = addition & 0xFFFF;
				O = (addition & 0xFFFF0000) >> 16;
				break;
			// Sub - 2 Cycle
			case 0x03:
				waitState++;
				*a = *a - *b;
				break;
			// Mul - 2 Cycle
			case 0x04:
				waitState++;
				*a = *a * *b;
				break;
			// Div - 2 Cycle
			case 0x05:
				waitState++;
				*a = *a / *b;
				break;
			// Mod - 2 Cycle
			case 0x06:
				unsigned int multiplication;
				waitState++;
				multiplication = *a * *b;
				*a = addition & 0xFFFF;
				O = (addition & 0xFFFF0000) >> 16;
			// SHL - 2 Cycle
			case 0x07:
				waitState++;
				*a = *a << *b;
				break;
			// SHR - 2 Cycle
			case 0x08:
				waitState++;
				*a = *a >> *b;
				break;
			// AND - 1 Cycle
			case 0x09:
				*a = *a & *b;
				break;
			// BOR - 1 Cycle
			case 0x0a:
				*a = *a | *b;
				break;
			// XOR - 1 Cycle
			case 0x0b:
				*a = *a ^ *b;
				break;
			// IFE - 2 Cycles
			case 0x0c:
				waitState++;
				if(*a != *b)
				{
					// If it fails, wait one cycle and skip next instruction.
					unsigned short a_test, b_test;
					a_test = (DCPU_Memory[PC] & 0x03F0) >> 4;
					b_test = (DCPU_Memory[PC] & 0xFC00) >> 10;

					if((a_test >= 0x10 && a_test <0x18) || a_test == 0x1E || a_test == 0x1F)
						PC++;

					if((b_test >= 0x10 && b_test <0x18) || b_test == 0x1E || b_test == 0x1F)
						PC++;

					PC++;
					waitState++;
				}
				break;
			// IFN - 2 Cycles
			case 0x0d:
				waitState++;
				if(*a == *b)
				{
					// If it fails, wait one cycle and skip next instruction.
					unsigned short a_test, b_test;
					a_test = (DCPU_Memory[PC] & 0x03F0) >> 4;
					b_test = (DCPU_Memory[PC] & 0xFC00) >> 10;

					if((a_test >= 0x10 && a_test <0x18) || a_test == 0x1E || a_test == 0x1F)
						PC++;

					if((b_test >= 0x10 && b_test <0x18) || b_test == 0x1E || b_test == 0x1F)
						PC++;

					PC++;
					waitState++;
				}
				break;
			// IFG - 2 Cycles
			case 0x0e:
				waitState++;
				if(*a <= *b)
				{
					// If it fails, wait one cycle and skip next instruction.
					unsigned short a_test, b_test;
					a_test = (DCPU_Memory[PC] & 0x03F0) >> 4;
					b_test = (DCPU_Memory[PC] & 0xFC00) >> 10;

					if((a_test >= 0x10 && a_test <0x18) || a_test == 0x1E || a_test == 0x1F)
						PC++;

					if((b_test >= 0x10 && b_test <0x18) || b_test == 0x1E || b_test == 0x1F)
						PC++;

					PC++;
					waitState++;
				}
				break;
			// IFB - 2 Cycles
			case 0x0f:
				waitState++;
				if((*a & *b) == 0)
				{
					// If it fails, wait one cycle and skip next instruction.
					unsigned short a_test, b_test;
					a_test = (DCPU_Memory[PC] & 0x03F0) >> 4;
					b_test = (DCPU_Memory[PC] & 0xFC00) >> 10;

					if((a_test >= 0x10 && a_test <0x18) || a_test == 0x1E || a_test == 0x1F)
						PC++;

					if((b_test >= 0x10 && b_test <0x18) || b_test == 0x1E || b_test == 0x1F)
						PC++;

					PC++;
					waitState++;
				}
				break;
		}
	} else {
		waitState--;
	}
}

