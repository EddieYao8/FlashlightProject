#define ledPin 9 // LED digital pin 9  ATtiny Pin 0
#define buttonPin 2  // Button Pin 2   ATtiny Pin 2
#define maxLED 255 // MAX LED level

int count = 0; // Store mode number
bool button = false; // Button press
int ledState = LOW;
int level_mid = maxLED/2;
int level_low = maxLED/4;

unsigned long previousMillis = 0; // store last time LED updated
unsigned long interval = 1000;  //interval at which to blink (milliseconds)
const long freq = 2; // Frequency in Hz
unsigned long currentMillis = 0; // Running time

int state = 0;

void setup() {
  //Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, LOW); 
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonstate, FALLING);
  //attachInterrupt(buttonPin,buttonstate, FALLING);
  interval = 500/freq;

}

void loop() {
  currentMillis = millis(); //Keep track of running time
  //Serial.println(currentMillis,DEC);

  checkbuttonpress(); // Check if button press 
  
  light_on();    //Change LED state
}

void buttonstate(){
 // state= digitalRead(buttonPin);
  //Serial.println("PUSH");
  button = true;
//  if (button = false){
//    Serial.println("FALSE");
//  }
  
}

void checkbuttonpress(){
  //button = false;
//  if (button == false){
//    Serial.println("FALSE");
//  }
  
  if (button == true){
    count = count +1;   // Change mode only when pressed
    //Serial.println(count);
    
    if (count>1000){
      count= 0;         // Limit on data size of int
    }
     button = false;
  }
//  Serial.println("CHECK");
 
}

void light_on(){
  // Function based on mode(count); CHANGE TO CASE STATEMENTS
  switch(count % 5) {
    case 0:
      analogWrite(ledPin, 0); // OFF
      break;
    
    case 1:
      analogWrite(ledPin, int(maxLED)); // High Mode
      break;
      
    case 2:
      analogWrite(ledPin, level_mid); // Mid Mode
      break;
      
    case 3:  
      analogWrite(ledPin, level_low); // Low Mode
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
        analogWrite(ledPin, maxLED); // On 
      } 
      else {
        ledState = LOW;
        analogWrite(ledPin, 0); // OFF
      }
    }
}
