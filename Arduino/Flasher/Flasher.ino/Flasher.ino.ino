#include <Arduino.h>

// We are going to use timer TC1 which has 16 bits and can have TOP set by UCR1 in CTC mode
// This timer is hardware keyed directly off the crystal, so rock steady no matter what is happening in Arduino land. 

// We will output on OCR1A, which is Arduino pin D9...
// http://www.pighixxx.com/test/wp-content/uploads/2017/05/uno.png



// We will use mode 14 1100 - FastPWM, ICR1 TOP

// We are shooting for a freq of about 10Hz with a clock of 16Mhz 
// With a precaller of 64 and a TOP of 65536 (maximum top), we get a freq of 3Hz. Just right.
// https://www.google.com/search?num=30&rlz=1C1CYCW_enUS687US687&ei=S7soWsHFBMKa_QaCvqyYDQ&q=%2816Mhz%29%2F64%2F65536&oq=%2816Mhz%29%2F64%2F65536&gs_l=psy-ab.3...12857.12857.0.12953.1.1.0.0.0.0.93.93.1.1.0....0...1c.1.64.psy-ab..0.0.0....0.1TkYycjOycA
// (Remeber, we lower the TOP to shorten the period)

// A TOP of 25000 gets us exactly 10Hz
// https://www.google.com/search?num=30&rlz=1C1CYCW_enUS687US687&ei=j7soWrGvKq2X_QbbsLzAAQ&q=%281%2F%2810Hz%29%2F%28%281%2F%2816Mhz%29%29*64%29&oq=%281%2F%2810Hz%29%2F%28%281%2F%2816Mhz%29%29*64%29&gs_l=psy-ab.3...8395.15378.0.15719.10.10.0.0.0.0.74.615.10.10.0....0...1c.1.64.psy-ab..0.0.0....0.viiUJFbRhIE

// A match of 2500 gets us a 10ms on time
// https://www.google.com/search?num=30&rlz=1C1CYCW_enUS687US687&ei=oLsoWpuXDuGHggfq_qzwAQ&q=%2810ms%29%2F%28%281%2F%2816Mhz%29%29*64%29&oq=%2810ms%29%2F%28%281%2F%2816Mhz%29%29*64%29&gs_l=psy-ab.3...46698.51048.0.51976.12.6.6.0.0.0.63.357.6.6.0....0...1c.1.64.psy-ab..0.0.0....0.Xhes5Xn-pOQ

// Define our center values for freqnecy 

#define CENTER_FREQ_HZ  10.0

// Define how much the dials can adjust 

#define FREQ_ADJUST_PCT 20.0

#define PRESCALER  64 
#define TIMER_TICKS_PER_S (F_CPU/PRESCALER)   /// Note that F_CPU is in ticks_per_s and ticks_per_second is smae as Hz

#define MS_PER_S 1000.0
#define TIMER_TICKS_PER_MS (TIMER_TICKS_PER_S/MS_PER_S)

void setup() {

  // Start with sane values here so we don'ty blow out the LEDs
  // but these will get rewritten very soon on 1st pass though loop()

  ICR1 = 25000; 
  OCR1A = 200;
  

  TCCR1A = 
  
      _BV( COM1A1 ) |   // Clear OC1A on match (non-inverting)
      _BV(WGM11)        // FastPWM, ICR1 TOP

  ;

  TCCR1B =
      _BV( WGM13) | _BV(WGM12) |     // Mode 14 - FastPWM, ICR1 TOP
      _BV( CS11)  | _BV(CS10)                   // Prescaller 64

  ;

  #if PRESCALER!=64
    #error PRESCALER #define must match actual bits in TCCR1B
  #endif


  
  ICR1 = 24197  ; 

  OCR1A = 2650;

  pinMode( 9 , OUTPUT);   // Set the DDR for the OC1A oputput pin.

  // Use the adjcent analog pins to drive the two sides of the potentiometer becuase, well, they are right there!

  pinMode( A0 , OUTPUT );
  pinMode( A2 , OUTPUT );
  digitalWrite( A2 , 1 );
  pinMode( A3 , OUTPUT );
  pinMode( A5 , OUTPUT );
  digitalWrite( A5 , 1 );

}

void loop() {
return;
  // Read -1.0 to +1.0 normalized values for the dial
  float freqDial = (analogRead( A4 ) -512)  /1023.0;

  // First adjust freqency...

  float adjustedFreqHz = CENTER_FREQ_HZ + ( ( FREQ_ADJUST_PCT / 100.0 ) * CENTER_FREQ_HZ * freqDial );

  // Compute the copesponding TOP value in ticks

  unsigned int top = (TIMER_TICKS_PER_S / adjustedFreqHz);

  unsigned int match = analogRead( A1 ) * 10;   // This gives us a range of 0-10230 ticks, which maps to 0 - 40ms on time 
  
  ICR1 = top; 

  OCR1A = match;

  //OCR1A = analogRead(A1);

   
}

