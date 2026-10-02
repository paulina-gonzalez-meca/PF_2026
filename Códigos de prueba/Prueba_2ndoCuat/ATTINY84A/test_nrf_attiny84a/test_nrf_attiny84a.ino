#include <SPI.h>
#include <RF24.h>
#include <avr/interrupt.h>

// --- Pin Definitions (ATTinyCore Arduino Pins) ---
#define PIN_PULSADOR 0 // PA0
#define PIN_LED      1 // PA1
#define PIN_IRQ      2 // PA2 (Interrupt pin from NRF, optional)
#define PIN_CE       2 // PA2 (Configured as CE in prompt)
#define PIN_SCK      4 // PA4
#define PIN_MISO     5 // PA5
#define PIN_MOSI     6 // PA6
#define PIN_CSN      7 // PA7

// Initialize NRF24L01 radio using CE and CSN pins
RF24 radio(PIN_CE, PIN_CSN);

// Address through which modules communicate (5 bytes)
const byte address[6] = "00001";

// Non-blocking timer variables
volatile uint32_t timer_ms = 0; // Incremented every 1ms by Timer1 ISR
uint32_t previous_tx_time = 0;
const uint32_t TX_INTERVAL_MS = 1000; // Send message every 1000 ms

// Payload message
const char message[] = "Hello ATtiny!";

// --- Timer1 Initialization (1ms ISR Tick) ---
void initTimer1() {
  cli(); // Disable global interrupts during setup

  // Set Timer1 to CTC (Clear Timer on Compare Match) Mode: WGM12 = 1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  // Set OCR1A for 1ms interrupt based on clock frequency
  // Assuming standard 8 MHz internal clock with Prescaler 64:
  // (8,000,000 / (64 * 1000)) - 1 = 124
  OCR1A = 124;

  TCCR1B |= (1 << WGM12);  // Enable CTC mode
  TCCR1B |= (1 << CS11) | (1 << CS10); // Prescaler = 64
  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 Compare Match A interrupt

  sei(); // Enable global interrupts
}

// --- Timer1 Interrupt Service Routine (Replaces ESP32 onTimer) ---
ISR(TIM1_COMPA_vect) {
  timer_ms++; // Increments once every 1 millisecond
}

// Custom millis replacement relying on Timer1 tick
uint32_t get_timer_ms() {
  uint32_t ms;
  cli(); // Atomic read to prevent corruption during ISR update
  ms = timer_ms;
  sei();
  return ms;
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_PULSADOR, INPUT_PULLUP);

  // Initialize hardware timer
  initTimer1();

  // Initialize NRF24L01 module
  if (radio.begin()) {
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_LOW); // Low power for ATtiny stabilization
    radio.stopListening();         // Set as transmitter
    
    // Quick blink to signal NRF initialization success
    digitalWrite(PIN_LED, HIGH);
  }
}

void loop() {
  uint32_t current_time = get_timer_ms();

  // Send payload periodically every 1 second without blocking delay()
  if (current_time - previous_tx_time >= TX_INTERVAL_MS) {
    previous_tx_time = current_time;

    // Pulse LED during transmit
    digitalWrite(PIN_LED, HIGH);
    
    // Transmit message over NRF
    bool success = radio.write(&message, sizeof(message));
    
    digitalWrite(PIN_LED, LOW);
  }

  // Handle immediate button interaction non-blockingly
  if (digitalRead(PIN_PULSADOR) == LOW) {
    // Optional instant send trigger on button press
  }
}