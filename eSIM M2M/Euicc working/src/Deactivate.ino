const int rstPin = 8;
const int ioPin = 7;
const int clkPin = 9;

void setup() {
  // Set pin modes
  pinMode(rstPin, OUTPUT);
  pinMode(ioPin, OUTPUT);
  pinMode(clkPin, OUTPUT);

  Serial.println("cheen tapak dum dum");

  
  // Generate 2 MHz clock signal on clkPin
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

  // Start sequence
  startSequence();

  readATR();

  

}

void startSequence() {
  // Step 1: Set rst high

  
  digitalWrite(rstPin, HIGH);
  
  // Step 2: Wait for a short period and then set rst low
  delayMicroseconds(10);
  digitalWrite(rstPin, LOW);

  // Step 3: Wait for 200 microseconds
  delayMicroseconds(200);

  digitalWrite(ioPin, HIGH);
  
  // Step 2: Wait for a short period and then set rst low
  delayMicroseconds(10);

    digitalWrite(ioPin, LOW);

  // Step 4: Stop the clock
  TCCR1B = 0;
  digitalWrite(clkPin, LOW);

  
  // Step 5: Wait for another 200 microseconds

  // Step 6: Set io pin to zero
}

void loop() {
  // No need to put any code in the loop

}

void readATR() {
  Serial.println("Reading ATR...");

  // Assuming ATR is 32 bytes max (adjust if needed)
  byte atr[32];
  int index = 0;

  // Read the ATR from the IO pin
  while (index < 32) {
    atr[index] = readByte();
    index++;
    delayMicroseconds(186);  // Adjust timing as per card's bit duration
  }

  // Print the ATR
  Serial.print("ATR: ");
  for (int i = 0; i < index; i++) {
    Serial.print(atr[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

byte readByte() {
  byte value = 0;
  for (int i = 0; i < 8; i++) {
    value <<= 1;
    if (digitalRead(ioPin) == HIGH) {
      value |= 1;
    }
    delayMicroseconds(186);  // Adjust timing as per card's bit duration
  }
  return value;
}


const int rstPin = 8;
const int ioPin = 7;
const int clkPin = 9;

void setup() {
  // Set pin modes
  

  Serial.begin(9600);
  Serial.println("Setup started");

 pinMode(rstPin, OUTPUT);
 pinMode(ioPin, INPUT);

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
  pinMode(9, OUTPUT);


   digitalWrite(rstPin, HIGH);
  
  // Step 2: Wait for a short period and then set rst low
  delayMicroseconds(10);
  digitalWrite(rstPin, LOW);

  // Step 3: Wait for 200 microseconds
  delayMicroseconds(200);

  digitalWrite(ioPin, HIGH);
  
  // Step 2: Wait for a short period and then set rst low
  delayMicroseconds(10);

    digitalWrite(ioPin, LOW);

  // Step 4: Stop the clock
  TCCR1B = 0;
  digitalWrite(clkPin, LOW);
  
  // Start generating the clock signal

  Serial.println("Setup completed");

  // Read ATR from eSIM
  readATR();
}

void loop() {
  // Send APDU command to the eSIM
}

  // Configure Timer1 to generate a clock signal


void readATR() {
  Serial.println("Reading ATR...");

  // Assuming ATR is 32 bytes max (adjust if needed)
  byte atr[32];
  int index = 0;

  // Read the ATR from the IO pin
  while (index < 32) {
    atr[index] = readByte();
    index++;
    delayMicroseconds(104);  // Adjust timing as per card's bit duration
  }

  // Print the ATR
  Serial.print("ATR: ");
  for (int i = 0; i < index; i++) {
    Serial.print(atr[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

byte readByte() {
  byte value = 0;
  for (int i = 0; i < 8; i++) {
    value <<= 1;
    if (digitalRead(ioPin) == HIGH) {
      value |= 1;
    }
    delayMicroseconds(186);  // Adjust timing as per card's bit duration
  }
  return value;
}
