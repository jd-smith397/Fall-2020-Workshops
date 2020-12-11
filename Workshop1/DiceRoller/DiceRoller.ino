const int BUTTON_PIN = 2; //Represents the pin connected to button
const int LED_PINS[] = {4, 7, 8, 11, 12, 13}; //An array which represents each LED pin
int rollNum; //Will store the current dice value based on a random roll

void setup() {
    pinMode(BUTTON_PIN, INPUT); //Sets the button pin as an input pin

    //Loop to set each LED pin in the array to an output pin
    for(int i = 0; i < 6; i++) {
        pinMode(LED_PINS[i], OUTPUT);
    }

    //Sets the seed for the pseudorandom function to the value of analog pin 0.
    randomSeed(analogRead(0));
    
}

void loop() {
    
    //When the button is pressed, the button pin input will read a logic 1
    //This will cause the code for the dice roll to occur
    if(digitalRead(BUTTON_PIN)) {

      //Rolls the dice, sets rollNum equal to a value 1-6
      rollNum = random(1,7);

      //Runs the transition animation of the LEDs
      transition();
     

      //Turns on the random number of LEDs
      for(int i = 0; i < rollNum; i++) {
        digitalWrite(LED_PINS[i], HIGH);
      }
    }
}


//A support function to create an animation
void transition() {
    for(int i = 0; i < 6; i++) {
        digitalWrite(LED_PINS[i], LOW);
    }

    for(int i = 0; i < 6; i++) {
        digitalWrite(LED_PINS[i], HIGH);

        //pause for 150 milli seconds
        delay(150);
    }

    for(int i = 0; i < 6; i++) {
        digitalWrite(LED_PINS[i], LOW);
        
    }

    //pause for 50 milli seconds
    delay(50);
}
