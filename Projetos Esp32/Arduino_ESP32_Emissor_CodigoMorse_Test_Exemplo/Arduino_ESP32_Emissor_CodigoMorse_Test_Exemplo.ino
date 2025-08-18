#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define relayout 5
#define relayin 18
#define speaker 17

// Define the keypad layout
const int ROWS = 4; // four rows
const int COLS = 4; // four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', '/'}
};
uint8_t rowPins[ROWS] = {27,13,16,4}; // connect to the row pinouts of the keypad
uint8_t colPins[COLS] = {33,25,26,14}; // connect to the column pinouts of the keypad

// Initialize the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Initialize the OLED display
#define OLED_RESET -1 // Reset pin for the OLED display
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);
// Morse code conversion table
const char* morseCode[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.",
  "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",
  ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----"
};
const int numChar = sizeof(morseCode) / sizeof(morseCode[0]);

// Buffer for storing the entered text
char buffer[100];
int bufferIndex = 0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Entre o Texto:");
  display.display();

  // Set up the relay pins
  pinMode(relayout, OUTPUT); // Relay for sending Morse code
  pinMode(relayin, INPUT_PULLUP);  // Relay for receiving Morse code
  pinMode(speaker, OUTPUT);  // Speaker for play sound
}

void loop() {
  // Handle keypad input
  char key = keypad.getKey();
  if (key) {
    if (key == '#') {
      // Convert and send Morse code
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Entre:");
      sendMorseCode(buffer);
      display.setCursor(0, 15);
      display.print("Morse Enviado: ");
      display.println(buffer);
      display.display();
      clearBuffer();
    } else if (key == '*') {
      // Convert received Morse code
      char receivedMorse[100];
      receiveMorseCode(receivedMorse);
      display.print("Recebido: ");
      display.println(receivedMorse);
      display.display();
      char receivedText[100];
      convertToText(receivedMorse, receivedText);
      display.print("Texto: ");
      display.println(receivedText);
      display.display();
    } else {
      // Add the key to the buffer
      buffer[bufferIndex++] = key;
      buffer[bufferIndex] = '\0';
      display.print(key);
      display.display();
    }
  }
}

// Function to send a single Morse code character
void sendMorseChar(const char* morseChar) {
  for (int i = 0; morseChar[i] != '\0'; i++) {
    if (morseChar[i] == '.') {
      digitalWrite(relayout, HIGH); // activate the relay
      digitalWrite(speaker, HIGH);
      tone(speaker, 850);
      delay(200);
      digitalWrite(relayout, LOW);  // deactivate the relay
      digitalWrite(speaker, LOW);
      noTone(speaker);
    } else if (morseChar[i] == '-') {
      digitalWrite(relayout, HIGH); // activate the relay
      digitalWrite(speaker, HIGH);
      tone(speaker, 850);
      delay(600);
      digitalWrite(relayout, LOW);  // deactivate the relay
      digitalWrite(speaker, LOW);
      noTone(speaker);
    }
    // Pause between dots and dashes
    delay(200);
  }
}

// Function to send Morse code using a relay
void sendMorseCode(const char* text) {
  for (int i = 0; text[i] != '\0'; i++) {
    char ch = toLowerCase(text[i]);
    if (ch >= 'a' && ch <= 'z') {
      int index = ch - 'a';
      sendMorseChar(morseCode[index]);
    } else if (ch >= '0' && ch <= '9') {
      int index = ch - '0' + 26;
      sendMorseChar(morseCode[index]);
    } else if (ch == '/') {
      // Pause between words
      delay(1400);
    }
    // Pause between characters
    delay(700);
  }
}


// Function to receive Morse code using a relay
void receiveMorseCode(char* receivedMorse) {
  int index = 0;
  while (digitalRead(relayin) == LOW) {
    // Wait for the relay to activate
  }
  unsigned long startTime = millis();
  while (digitalRead(relayin) == HIGH) {
    // Wait for the relay to deactivate
  }
  unsigned long endTime = millis();
  unsigned long duration = endTime - startTime;

  if (duration >= 50 && duration <= 300) {
    receivedMorse[index++] = '.';
  } else if (duration >= 300 && duration <= 900) {
    receivedMorse[index++] = '-';
  }
  receivedMorse[index] = '\0';
}

// Function to convert Morse code back to text
void convertToText(const char* receivedMorse, char* receivedText) {
  int index = 0;
  char tempMorse[100];
  strcpy(tempMorse, receivedMorse);  // Copy the content of receivedMorse to tempMorse

  char* token = strtok(tempMorse, " ");
  while (token != NULL) {
    for (int i = 0; i < numChar; i++) {
      if (strcmp(token, morseCode[i]) == 0) {
        if (i < 26) {
          receivedText[index] = i + 'A';
        } else {
          receivedText[index] = i - 26 + '0';
        }
        index++;
        break;
      }
    }
    token = strtok(NULL, " ");
  }
  receivedText[index] = '\0';
}

// Function to clear the buffer
void clearBuffer() {
  for (int i = 0; i < bufferIndex; i++) {
    buffer[i] = '\0';
  }
  bufferIndex = 0;
}
