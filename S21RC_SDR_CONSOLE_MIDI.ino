/* 
 * USB CONTROLLER FOR SDR CONSOLE
 * Written by S21RC Fazlay Rabby S21RC
 * For using as an External VFO and PTT control
 * Version: 1.0
 * 
 * 
 * A large PTT button is used for PTT
 * A rotary encoder with two mode has been achiieved by the push switch of the encoder, or an extra momentary push switch can be used. 
 * I have used default mode as frequency fine tune up/down, other mode for normal tune up/down
 * You can configure the action from the SDR CONSOLE menu (tools>optiions>Controllers>MIDI)
 * 
 * Boards: Leonardo, Micro, Pro Micro (native USB supported board).
 *
 *
 * Connections
 * -----------
 * 
 * - 2: pin A of the encoder
 * - 3: pin B of the encoder
 * - 4: switch pin of the encoder
 * - Encoder common pin to GND
 * - 5: PTT switch pin (connect other side of the ptt switch to ground)
 * - 6: Anode of Red LED with a 470R resistor in series.
 * - 7: Anode of Blue LED with a 470R resistor in series.
 
 * This code usage control surface library:
 * 
 * Written by PieterP, 2019-12-18  
 * https://github.com/tttapa/Control-Surface
 */
#include <MIDIUSB.h>
#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

int LED_RED = 6;
int LED_BLUE = 7;
unsigned long heartbit = 0;

Bank<2> bank;
// Selector to change banks
IncrementSelector<2> selector = {
  bank,       // Bank to manage
  4,          // Rotary encoder push button pin
};

// Instantiate a CCRotaryEncoder object
Bankable::CCRotaryEncoder enc = {
  {bank, BankType::CHANGE_ADDRESS},     // Bank configuration
  {2, 3}, //pins of rotary encoder
  {0x01}, // midi cc adress
  1,     //multiplier
  1,     //4pulses per step
};


// Instantiate a CCButton object
CCButton button {
  // PTT button on pin 5:
  5,
  // General Purpose Controller #1 on MIDI channel 1:
  {MIDI_CC::General_Purpose_Controller_1, Channel_1},
};



void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  RelativeCCSender::setMode(relativeCCmode::MACKIE_CONTROL_RELATIVE);
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface

//Bank selection  0 = no led, 1 = RED_LED
  if (bank.getSelection()) digitalWrite(LED_RED, HIGH);
  else digitalWrite(LED_RED, LOW);

// Heartbit LED blinking  
  if (millis() - heartbit > 500) {
    digitalWrite(LED_BLUE, LOW);
    heartbit = millis(); 
  }
    //Pin 11 turns on every second
  else if (millis() - heartbit > 200) {
    digitalWrite(LED_BLUE, HIGH);
  }
}
