/**
 * IR Transmitter for the AtTiny85 using Timer0 in CTC mode to generate a Canon DLSR remote IR pulse.
 */

#define F_CPU 8000000
#define F_CPU_CTP 0.000125

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

long pulse = 0;
long pulse_tc = 0;

/** 
 * A single pulse for a 38Khz IR detector is 26 microseconds(μs) long - 
 * or 13 microseconds(μs) ON and 13 microseconds(μs) OFF.
 *
 * The Timer0 interrupt will happen every 13μs and the compare output
 * mode will toggle the logical level of pin OC0A (PB0).
 *
 * For every interrupt we check to see if the required duration for the 
 * IR remote ON code was reached. If it has then reset the pulse counter
 * and issue a long delay to signal an OFF code.
 */
ISR(TIM0_COMPA_vect)
{
	if (pulse < pulse_tc)
	{
		pulse += 13;
	}
	else
	{
		pulse = 0;	

		// This is a very important piece of code.
		// It is difficult to say exactly without an oscilloscope how the 
		// toggle mode is affecting this  but it would appear that the OCOA 
		// pin which is connected  to the OCR0A register will end in a logical 
		// HIGH which means the pin PB0 will stay HIGH. So the delay here has no effect 
		// in the IR pulse and instead it stays on. So we need to forcefully 
		// switch it off and then reset TOP to the clock ticks needed to produce
		// a 38Khz signal. An easy way to verify this is to connect a normal LED to 
		// PB0 and see if it switches off for the 3ms delay.
		// 
		// The end state of the OCR0A register could be something to do with the 
		// conditional above in that it evaluates to false before the waveform hits
		// a logical LOW.
		OCR0A = 0;
		
		_delay_ms(3000);
		PORTB ^= (1 << PB1);
		_delay_ms(500);
		PORTB ^= (1 << PB1);

		// Reset TOP 
		OCR0A = 103;
	} 
}   

int main (void)
{
	// Set port to output 
	DDRB |= (1 << PB0);
	DDRB |= (1 << PB1);

	// Enable internal pull-ups
	PORTB = (0 << PB0);
	PORTB = (0 << PB1);

	// Enable Waveform Generation mode - CTC mode 
	TCCR0A = (1 << WGM01);

	// Compare output mode set to Toggle
	// Toggle mode will switch between its logical 
	// level each time a compare match occurs.
	TCCR0A |= (1 << COM0A0);

	// Set up timer 0 with no prescaling 
	TCCR0B |= (1 << CS00);

	// Timer Compare value
	OCR0A = 103;

	// Enable Timer0 Compare Interrupt
	TIMSK |= (1 << OCIE0A);

	// Initialize Timer0
	TCNT0 = 0; 

	// Calculate the total amount of pulses needed to 
	// accurately to emit the IR code. The Canon remote has 
	// an on pulse for 280μs on and then off for any long ms duration
	pulse_tc = ((0.280 / F_CPU_CTP)) / 2 - 1;

	// Enable global interrupts
	sei();
  
	while(1)
	{	   

	}
	return 0;
}