#include <avr/io.h>                                         // AVR Interface with the hardware [2]
#include <avr/interrupt.h>                                   // AVR Interrupt library [3]
#include <avr/sleep.h>                                      // AVR Sleep-mode library [4]

#define maxLED 255 // MAX LED level
int LED = 0; // Attiny PB0
int ledState = LOW;
int level_mid = maxLED/2;
int level_low = maxLED/4;

int buttonPin = 3; // ATtiny PB3
volatile bool button = false; // Interrupt Button press
int count = 0;
int state = 0;
unsigned long interval = 1000;  //interval at which to blink (milliseconds)
const long freq = 2; // Frequency in Hz

unsigned long previousMillis = 0; // store last time LED updated
unsigned long currentMillis = 0; // Running time


void setup() {
  cli();
  pinMode(LED,OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, LOW); 
  
  PORTB &= ~((1 << LED)|(1 << buttonPin));    // Internal Pull-up is disabled for the nonfloating I/O pins (IPIN and OPIN)
  DDRB  |= (1 << LED);                   // Set OPIN to output (this is the LED Driver)
  // interrupt Initialization 
  GIMSK |= 0x20;                          // Global staging: Turns on pin interrupt capability 
  PCMSK |= (1 << buttonPin);                   // Set IPIN to input-- interrupt (this is debounced button)
  
 // Enable interrupt
  sei();
  interval = 500/freq;
}

void loop() {
  // put your main code here, to run repeatedly:
  currentMillis = millis();
  checkpress();
  
  light_on(); // LED state
}

void checkpress(){
  if (button == true){
    count = count +1;   // Change mode only when pressed  
    if (count>1000){
      count= 0;         // Limit on data size of int
    }
     button = false;    // reset bool
  }
}

void light_on(){
  switch(count % 5) {
    case 0:
      analogWrite(LED, 0); // OFF
      break;
      
    case 1:  
      analogWrite(LED, level_low); // Low Mode
      break;
    
    case 2:
      analogWrite(LED, level_mid); // Mid Mode
      break;
      
   case 3:
      analogWrite(LED, int(maxLED)); // High Mode
      break;
      
    case 4: 
      blinkLED();
      break;
  }
}

void blinkLED(){
    // Blinking at 2Hz
    if (currentMillis - previousMillis >= interval){
      previousMillis = currentMillis; // save the last time you blinked the LED

      if (ledState == LOW){
        ledState = HIGH;
        analogWrite(LED, maxLED); // On 
      } 
      else {
        ledState = LOW;
        analogWrite(LED, 0); // OFF
      }
    }
}
ISR(PCINT0_vect) {                                            // Respond on pin change (Rise | Fall)
  if (!digitalRead(buttonPin)) {                                    // ~High Respond -> Falling Edge
    button |= true;                                            // Turn on swtch state
  }
}
