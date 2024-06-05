# flow_sensor_test
flow sensors are mainly used to help use have a touch on flow rates of certain fluids. in our project we need to determine the flow rate of our cryogenic liquid LOX  and Methane gas as fuel 
Breakdown and Explanation of the   Code:
This code measures the flow rate of a liquid using a flow sensor and displays the results on an LCD and serial monitor. Here's a detailed explanation of each section:
1. Libraries and Pin Definitions:
•	#include <LiquidCrystal.h>: This line includes the LiquidCrystal library needed to control the LCD display.
•	LiquidCrystal lcd(7, 8, 9, 10, 11, 12);: This line creates an object called lcd and defines the six pins connected to the LCD (replace these with the actual pins if different).
•	#define FLOWSENSORPIN 2: This line defines a constant FLOWSENSORPIN set to the value 2, indicating the pin connected to the flow sensor (change this if the sensor is on a different pin).
2. Variables for Flow Measurement:
•	volatile uint16_t pulses = 0;: This variable pulses keeps track of the total number of pulses received from the flow sensor, initialized to 0.
•	volatile uint8_t lastflowpinstate;: This variable lastflowpinstate stores the previous state of the flow sensor pin (HIGH or LOW).
•	volatile uint32_t lastflowratetimer = 0;: This variable lastflowratetimer keeps track of the time elapsed (in milliseconds) since the last pulse was detected, initialized to 0.
•	volatile float flowrate;: This variable flowrate stores the calculated flow rate in Hertz (pulses per second).
3. Interrupt Service Routine (ISR): timer0Interrupt():
•	This function is called every millisecond by the timer interrupt on Timer/Counter 0 (assuming an AVR board).
•	uint8_t x = digitalRead(FLOWSENSORPIN);: This line reads the current state of the flow sensor pin and stores it in x.
•	if (x == lastflowpinstate) { ... }: This block checks if the current state is the same as the previous one. If so, it means there was no pulse, so lastflowratetimer is incremented, and the function returns.
•	if (x == HIGH) { ... }: This block checks if the current state is HIGH (indicating a pulse).
o	flowrate = 1000.0 / (lastflowratetimer + 1);: This line calculates the flow rate by dividing 1000 milliseconds (1 second) by the time elapsed since the last pulse (lastflowratetimer + 1 to avoid division by zero).
o	lastflowratetimer = 0;: Resets the timer for the next pulse.
•	lastflowpinstate = x;: Updates lastflowpinstate with the current state for comparison in the next interrupt.
4. Interrupt Control Function: useInterrupt(boolean v):
•	#ifdef __AVR__: This check ensures the code is compiled for AVR architecture (adjust for other boards).
•	if (v) { ... } else { ... }: This block enables or disables the interrupt based on the v parameter (true for enable, false for disable).
o	OCR0A = 0xAF;: Sets the Timer/Counter 0 Compare Match A register value (might need adjustment depending on your desired interrupt frequency).
o	TIMSK0 |= _BV(OCIE0A);: Enables the Timer/Counter 0 Compare Match A interrupt.
o	TIMSK0 &= ~_BV(OCIE0A);: Disables the Timer/Counter 0 Compare Match A interrupt.
5. Setup Function:
•	Serial.begin(9600);: Initializes serial communication at 9600 baud rate for debugging.
•	Serial.print("Flow sensor test!");: Prints a message to the serial monitor.
•	lcd.begin(16, 2);: Initializes the LCD display with 16 columns and 2 rows.
•	pinMode(FLOWSENSORPIN, INPUT);: Sets the flow sensor pin as an input.
•	digitalWrite(FLOWSENSORPIN, HIGH);: Sets the initial state of the flow sensor pin to HIGH (might not be necessary for all sensors).
•	lastflowpinstate = digitalRead(FLOWSENSORPIN);: Reads the initial state of the flow sensor pin.
•	useInterrupt(true);: Enables the interrupt for flow rate measurement.
6. Loop Function:
•	This function runs repeatedly.
•	Updates the LCD display:
o	lcd.setCursor(0, 0);: Sets the cursor position on the LCD

