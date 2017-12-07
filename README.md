# troxes-zoe-flasher

This is a hacked together ultra violet flasher for the Troxes x-mas display in the New Museum window.

# Light bar hardware

It uses these UV light bars...

http://amzn.to/2iY71oz

Which have been modified to allow them to be strobed and to run at a lower voltage.

The included power supply is constant current, so that will not do.

I used 27 ohm resistors for current limits on each LED gang because that's what I had. The LEDs have a forward voltage of 3.3V, so a gang of 4 has a Vf of 13.2V. At 100mA, we will have a total voltage drop of 13.2V + (0.1A * 27 Ohm) = 15.9V. Based on thier size, I'm just guessing that these LEDs can take 100mA in short bursts. Maybe much more. Should really check how much current they got on the included supply.

 

To switch the UV LEDs on, we use IRLB8721 N-chan MOSFETS on the low side of the ganged LEDs.  
