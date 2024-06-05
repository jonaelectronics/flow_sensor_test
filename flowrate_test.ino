/**********************************************************
This is example code for using liquid flow meters. 
  Connect the red wire to +5V, 
the black wire to common ground 
and the yellow sensor wire to pin #2
  **********************************************************/
#include <LiquidCrystal.h> // Include the LiquidCrystal library for LCD

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);  // Define LCD pin connections

#define FLOWSENSORPIN 2  // Pin connected to the flow sensor

// Variables for flow measurement
volatile uint16_t pulses = 0;
volatile uint8_t lastflowpinstate;
volatile uint32_t lastflowratetimer = 0;
volatile float flowrate;

// Interrupt service routine for Timer/Counter 0 compare match A (assuming AVR board)
void timer0Interrupt() {
  uint8_t x = digitalRead(FLOWSENSORPIN);

  if (x == lastflowpinstate) {
    lastflowratetimer++;
    return; // Nothing changed
  }

  if (x == HIGH) { // Pulse detected (LOW to HIGH transition)
    // Calculate flow rate only when a pulse is detected
    flowrate = 1000.0 / (lastflowratetimer + 1); // Avoid division by zero
    lastflowratetimer = 0;
  }
  lastflowpinstate = x;
}

void useInterrupt(boolean v) {
  #ifdef __AVR__  // Check if compiling for AVR architecture
  if (v) {
    // Timer0 is already used for millis() - interrupt on compare match A
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // Disable interrupt on compare match A
    TIMSK0 &= ~_BV(OCIE0A);
  }
  #endif
}

void setup() {
  Serial.begin(9600);
  Serial.print("Flow sensor test!");
  lcd.begin(16, 2);  // Initialize LCD with 16 columns and 2 rows

  pinMode(FLOWSENSORPIN, INPUT);
  digitalWrite(FLOWSENSORPIN, HIGH);
  lastflowpinstate = digitalRead(FLOWSENSORPIN);
  useInterrupt(true);  // Enable interrupt
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Pulses:");
  lcd.print(pulses, DEC);
  lcd.print(" Hz:");
  lcd.print(flowrate);

  Serial.print("Freq: ");
  Serial.println(flowrate);
  Serial.print("Pulses: ");
  Serial.println(pulses, DEC);

  // Flow rate calculation for plastic sensor (uncommented)
  float liters = pulses / (7.5 * 60.0);  // Liters per minute
  /*
  // if a brass sensor use the following calculation
  float liters = pulses;
  liters /= 8.1;
  liters -= 6;
  liters /= 60.0;
*/

  Serial.print(liters);
  Serial.println(" Liters");
  lcd.setCursor(0, 1);
  lcd.print(liters);
  lcd.print(" Liters    ");

  delay(100);
}
