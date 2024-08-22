#define RST_PIN 8  // Define the RST pin
#define IO_PIN 7   // Define the IO pin
#define CLK_PIN 9  // Define the clock pin
#define VCC_PIN 10 // Define the VCC pin

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);
  Serial.println("Cheen TAPPPAK DUM Dummmmm");

  // Set the pins as output or input as needed
  pinMode(CLK_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(IO_PIN, INPUT);
  pinMode(VCC_PIN, OUTPUT);

  // Initially set the RST and VCC pins to LOW
  digitalWrite(RST_PIN, LOW);
  digitalWrite(VCC_PIN, LOW);

  // Keep the RST and VCC pins low for 5 seconds
  delay(5000);

  // Power on the VCC pin
  digitalWrite(VCC_PIN, HIGH);

  // Wait for 400 clock cycles (200 microseconds)
  delayMicroseconds(200);

  // Set the RST pin to HIGH
  digitalWrite(RST_PIN, HIGH);

  // Start generating the clock signal
  startClock();

  Serial.println("Setup completed");

  // Read ATR from eSIM
  readATR();
}

void loop() {
  // Nothing to do in the loop
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

void readATR() {
  Serial.println("Reading ATR...");

  // Wait for the IO pin to go low
  while (digitalRead(IO_PIN) == HIGH);

  // Skip the first low bit (186.5 microseconds)
  delayMicroseconds(186);

  // Read the first 8 bits (1 byte) of the ATR
  byte firstByte = readByte();

  // Reverse bits
  byte reversedByte = reverseBits(firstByte);

  // Print the first byte of the ATR
  Serial.print("First ATR Byte: ");
  Serial.print(firstByte, HEX);
  Serial.print(" (original), ");
  Serial.print(reversedByte, HEX);
  Serial.println(" (reversed)");
}

byte readByte() {
  byte value = 0;
  for (int i = 0; i < 8; i++) {
    value <<= 1;
    if (digitalRead(IO_PIN) == HIGH) {
      value |= 1;
    }
    delayMicroseconds(186);  // 1 ETU = 186.5 microseconds
  }
  return value;
}

byte reverseBits(byte b) {
  byte reversed = 0;
  for (int i = 0; i < 8; i++) {
    reversed <<= 1;
    if (b & 1) {
      reversed |= 1;
    }
    b >>= 1;
  }
  return reversed;
}

