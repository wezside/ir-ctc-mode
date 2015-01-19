IR Transmitter for the AtTiny85 using Timer0 in CTC mode to generate a Canon DLSR remote IR pulse.

About
-----

It works by setting the Timer0 in CTC mode using a waveform generator to produce a 38Khz IR pulse. The waveform generation mode is set to toggle. Using a Timer interrupt the pulse duration is counted and a delay is added once duration has been reached to signal the end of IR transmission. 

I'm using a Yongnu Digital Canon Remote with the following codes:

280μs - 300μs ON
1000ms+ 	  OFF

The vars *pulse* and *pulse_tc* is measured as Clock Time Period (CTP). The IR code 280μs ON is converted to CTP using hte formula:

timer_count = Required Delay / Clcok Time Period  - 1


Resources
---------

* AtTiny85 Datasheet
* [MAxEmbedded AVR Timer CTC Timer Tutorial](http://maxembedded.com/2011/07/avr-timers-ctc-mode/)