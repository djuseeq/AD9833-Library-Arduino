/*
 * AD9833.h
 * 
 * Copyright 2016 Bill Williams <wlwilliams1952@gmail.com, github/BillWilliams1952>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#ifndef __AD9833__

#define __AD9833__

#include <Arduino.h>
#include <SPI.h>

#define pow2_28		268435456L	// 2^28 used in frequency word calculation

#define RESET_CMD		0x0100		// Control write, reset enabled
#define FREQ0_WRITE		0x4000
#define FREQ1_WRITE		0x8000
#define PHASE1_REG		0x0400
#define FREQ1_REG		0x0800

/*
 * Initial settings for the control register for waveform type.
 * Control write (D15 D14 = 00)
 * 		Output using FREQ0 for frequency and PHASE0 for phase
 * 		Reset disabled
 * 		DAC output active (SINE/TRIANGLE)/in sleep (SQUARE) based on waveform
 * 		Internal clock enabled.
 * 		Either a SINE, TRIANGLE, SQUARE, or HALF_SQUARE waveform
 * There seems to be a bug here.  I would assume that a control write does
 * not need Bit D13 set.  This seems to be true for ALL waveforms except
 * SINE_WAVE.  Why, I don't know.
 */
typedef enum { SINE_WAVE = 0x2000, TRIANGLE_WAVE = 0x0002,
			   SQUARE_WAVE = 0x0068, HALF_SQUARE_WAVE = 0x0060,
			   CURRENT_WAVEFORM = 0 } WaveformType;
			   
typedef enum { REG0, REG1, SAME_AS_REG0 } Registers;

class AD9833 {

public:
	
	AD9833 ( uint8_t FNCpin, uint32_t referenceFrequency = 25000000UL );

	// Must be the first command after creating the AD9833 object.
	void Begin ( void );

	// Reset output
	void Reset ( void );

	// Update just the frequency in REG0 or REG1
	void SetFrequency ( Registers freqReg, float frequency );

	// Update just the phase in REG0 or REG1
	void SetPhase ( Registers phaseReg, float phase );

	// SINE_WAVE, TRIANGLE_WAVE, SQUARE_WAVE, HALF_SQUARE_WAVE,
	// CURRENT_WAVEFORM
	void SetWaveform ( WaveformType waveType );

	// Output based on the contents of REG0 or REG1
	void SetOutputSource ( Registers freqReg, Registers phaseReg = SAME_AS_REG0 );

	// Turn ON / OFF output
	void EnableOutput ( bool enable );

	// TODO:
	void SetupSignal ( Registers freqReg, float frequency, Registers phaseReg,
						float phase, WaveformType waveType );

	// Increment the selected frequency register by freqIncHz
	void IncrementFrequency ( Registers freqReg, float freqIncHz );

private:

	void 			WriteRegister ( int16_t dat );
	void 			WriteControlRegister ( void );
	uint16_t		waveForm, phase0, phase1;
	uint8_t			FNCpin, outputEnabled;
	uint32_t		refFrequency;
	float			frequency0, frequency1;
	Registers		activeFreq, activePhase;
};

#endif
