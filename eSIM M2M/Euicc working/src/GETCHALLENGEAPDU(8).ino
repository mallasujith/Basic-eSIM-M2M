0x00, 0x84, 0x00, 0x00, 0x08


#include <avr/io.h>
#include <Arduino.h>

#define RST_PIN 8  // Define the RST pin
#define IO_PIN 7   // Define the IO pin
#define CLK_PIN 9  // Define the clock pin
#define VCC_PIN 10 // Define the VCC pin

// Define the APDU command you want to send
const uint8_t apduCommand[] = {
0x00, 0x84, 0x00, 0x00, 0x08
};

const unsigned int etu = 186; // Define ETU as 186 microseconds

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  initialize();
  digitalWrite(2, HIGH);
  waitForATR();
  delay(100);
  digitalWrite(2, LOW);
  sendAPDU();
  digitalWrite(2, HIGH);
  receiveAPDU();
}

void loop() {
  // Main loop can be empty as we are doing everything in setup for this example
}

void initialize() {
  // Set the pins as output
  pinMode(CLK_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(IO_PIN, OUTPUT);
  pinMode(VCC_PIN, OUTPUT);

  // Initially set all pins to LOW
  digitalWrite(CLK_PIN, LOW);
  digitalWrite(RST_PIN, LOW);
  digitalWrite(IO_PIN, LOW);
  digitalWrite(VCC_PIN, LOW);

  // Keep all pins low for 5 seconds
  delay(5000);

  // Power on the VCC pin
  digitalWrite(VCC_PIN, HIGH);

  // Generate the clock signal
  startClock();

  // Wait for a short time before changing IO pin mode
  delayMicroseconds(70);
  pinMode(IO_PIN, INPUT);

  // Keep RST low for 140us (time to wait before setting RST high)
  delayMicroseconds(140);

  // Set RST high
  digitalWrite(RST_PIN, HIGH);

  // Wait for ATR (Assuming the card is now ready for communication)
  //delay(50); // 50 milliseconds to ensure the card is ready
}

void waitForATR() {
}

void sendAPDU() {
  pinMode(IO_PIN, OUTPUT);

  // Send APDU command
  for (uint8_t i = 0; i < sizeof(apduCommand); i++) {
    sendByte(apduCommand[i]);
  }
}

void receiveAPDU() {
  pinMode(IO_PIN, INPUT);
  // Implement reading logic here to receive data from the eSIM
}

void startClock() {
  // Configure Timer1 to generate a clock signal
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // Toggle OC1A on Compare Match
  TCCR1A |= (1 << COM1A0);

  // CTC mode
  TCCR1B |= (1 << WGM12);

  // Set compare match register for 2MHz (assuming 16MHz clock)
  OCR1A = 3;

  // No prescaler
  TCCR1B |= (1 << CS10);

  // Set the pin mode for the clock output
  pinMode(CLK_PIN, OUTPUT);
}

void sendByte(uint8_t data) {
  uint8_t parity = calculateParity(data);

  // Send the start bit (always low)
  digitalWrite(IO_PIN, LOW);
  delayMicroseconds(etu);  // Delay for one ETU (186 us)

  // Send data bits
  for (int i = 0; i < 8; i++) {
    bool bit = (data >> i) & 0x01;
    digitalWrite(IO_PIN, bit);
    delayMicroseconds(etu);  // Delay for one ETU (186 us)
  }

  // Send parity bit
  digitalWrite(IO_PIN, parity);
  delayMicroseconds(etu);  // Delay for one ETU (186 us)

  // Send stop bits (always high)
  digitalWrite(IO_PIN, HIGH);
  delayMicroseconds(etu);  // Delay for one ETU (186 us)
  digitalWrite(IO_PIN, HIGH);
  delayMicroseconds(etu);  // Delay for one ETU (186 us)

}

uint8_t calculateParity(uint8_t data) {
  uint8_t onesCount = 0;
  for (int i = 0; i < 8; i++) {
    onesCount += (data >> i) & 0x01;
  }
  return (onesCount % 2 == 1) ? HIGH : LOW;
}
