IR Transmitter for the AtTiny85 using Timer0 in CTC mode to generate a Canon DLSR remote IR pulse.

About
-----

It works by setting the Timer0 in CTC mode using a waveform generator to produce a 38Khz IR pulse. The waveform generation mode is set to toggle. Using an Timer interrupt the pulse duration is counted and a break is added to signal the end of IR transmission. 

I'm using a Yongnu Digital Canon Remote with the following codes:

280μs - 300μs ON
1000ms+ 	  OFF