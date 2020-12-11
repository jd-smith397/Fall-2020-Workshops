#include <HCSR04.h>


#define ECHO_pin 9
#define TRIG_pin 10


//Struct to keep track of everything associated with the blinking LED
struct LED_Timer {
  //previousMillis & currentMillis will be used to time the LED's blinks rather than a delay
  unsigned long previousMillis = 0; 
  unsigned long currentMillis;
  
  //toggle interval is the minimum # of microseconds before the LED toggles to the opposite state
  long toggle_interval = 500;

  //The state of the LED
  int LED_state = LOW; 
  int LED_pin;
};

//Struct to keep track of everything associated with timing the distance data sampling and serial printing
struct poll_data_timer {
  //These variables serve the same function as those of the same name in the LED_Timer
  unsigned long previousMillis = 0;
  unsigned long currentMillis;

  //Serves the same function as the toggle_interval, but for polling distance data
  long poll_interval;
};

//Declaring object from HCSR04 Library
UltraSonicDistanceSensor ultra_sonic(TRIG_pin, ECHO_pin);

//Distance from the sensor
int distance;

//Declaring led object
LED_Timer led;
LED_Timer builtin;

//Declaring two timers
poll_data_timer sensor_timer;
poll_data_timer serial_timer;

void setup() {
  //Initializes the LED struct
  init_LED(led,2);
  init_LED(builtin, LED_BUILTIN);

  //Sets the timing for the sensor to 50 ms
  sensor_timer.poll_interval = 50;

  //Sets the serial baud rate to 115200 baud
  Serial.begin(115200);

  //Sets the timing for serial print to 5s
  serial_timer.poll_interval = 5000;
}

void loop() {

  //Block checks if more than 50 ms have passed, if it has it will poll distance data
  sensor_timer.currentMillis = millis();
  if(sensor_timer.currentMillis - sensor_timer.previousMillis >= sensor_timer.poll_interval) {
    //Utilizing the object from the HRS04 with its member function measureDistanceCm,
    //  which returns the distance value in cm
    distance = ultra_sonic.measureDistanceCm();
    sensor_timer.previousMillis = sensor_timer.currentMillis;

    //If the object is farther than 1 meter, the LED will be set to LOW until the object is within
    //  1 meter.
    if(distance > 100) {
      digitalWrite(led.LED_pin, LOW);
      led.LED_state = LOW;
      
      //Interval value zero used as a flag for no blink
      led.toggle_interval = 0; 
    }
    //There is a blink interval for every discrete cm between 10 & 100
    else if(distance <= 100 && distance > 10){
      update_toggle_interval(led, distance * 20);
      update_toggle_interval(led, distance * 10);
    }
    //Blink interval constant from 0 cm to 10 cm
    else {
      update_toggle_interval(led, 200);
      update_toggle_interval(builtin, 100);  
    } 

  }

  //The value 0 for toggle_interval is used as a flag for when the LED should keep its state at LOW
  //If the toggle_interval is not 0, the LED state will be updated
  if(led.toggle_interval != 0) {
    update_LED_state(led);
    update_LED_state(builtin);
  }

  //Block which prints the distance to serial every 5 seconds
  serial_timer.currentMillis = millis();
  if(serial_timer.currentMillis - serial_timer.previousMillis >= serial_timer.poll_interval) {
    serial_timer.previousMillis = serial_timer.currentMillis;
    Serial.println("Distance: " + String(distance) + " cm");
  }
  
}

//Function to update the interval of the led object which is passed
void update_toggle_interval(LED_Timer& led, int newInterval) {
  led.toggle_interval = newInterval;
}


//Function to update the state of the led object passed by reference based on its timer
void update_LED_state(LED_Timer& led) {
    //Measures the number of ms which have passed since the Arduino started up
    led.currentMillis = millis();

    //If the number of millseconds that have passed since the last update is greater than or equal to the interval
    if(led.currentMillis - led.previousMillis >= (led.toggle_interval)) {

      //Once in the if-block the currentMillis is assigned to the previousMillis for the next update
      led.previousMillis = led.currentMillis;
      
      //Toggles the state of the LED
      led.LED_state = ((led.LED_state) + 1) % 2;
      //led.LED_state = ((led.LED_state) == HIGH) ? LOW:HIGH; 
      
      //Finally, sets the pin to the LED's new state
      digitalWrite(led.LED_pin, led.LED_state);
    }
  }

//Function to set up an LED
void init_LED(LED_Timer& led, int LED_pin){
  led.LED_pin = LED_pin;
  pinMode(LED_pin, OUTPUT); 
}
