#include <EEPROM.h>
#include <Keyboard.h>

#define KBD_RELEASE 0xE0
#define KBD_RELEASE_ALL 0xE1
#define KBD_AUTORELEASE_ON 0xE2
#define KBD_AUTORELEASE_OFF 0xE3
#define KBD_CLICK_LENGTH 0xE4

#define CMD_DELAY 0xF0
#define CMD_DELAY_TEN 0xF1
#define CMD_EXEC 0xF4
#define CMD_PROGRAM 0xF5
#define CMD_DUMP 0xF6
#define CMD_NOOP 0xFD
#define CMD_BEGIN 0xFE
#define CMD_END 0xFF

void program() {
  for (int i=0; i < EEPROM.length(); i++) {
    int c = int(Serial.read());
    EEPROM.write(i, char(c));
    switch(c) {
    case CMD_END: return;
    default: break;
    }
  }
}

void dump_program() {
  for (int i=0; i < EEPROM.length(); i++) {
    int c = int(EEPROM.read(i));
    Serial.write(char(c));
    switch(c) {
    case CMD_END: return;
    default: break;
    }
  }
}

void dump_eeprom() {
  for (int i=0; i < EEPROM.length(); i++) {
    Serial.write(EEPROM.read(i));
  }
}

void execute() {
  boolean autorelease = false;
  int click_length = 0;
  Keyboard.begin();
  for (int i=0; i < EEPROM.length(); i++){
    int c = int(EEPROM.read(i));
    switch(c) {
    case KBD_RELEASE:
      Keyboard.release(char(EEPROM.read(++i))); break;
    case KBD_RELEASE_ALL:
      Keyboard.releaseAll(); break;
    case KBD_AUTORELEASE_ON:
      autorelease = true; break;
    case KBD_AUTORELEASE_OFF:
      autorelease = false; break;
    case KBD_CLICK_LENGTH:
      click_length = int(EEPROM.read(++i)); break;
    case CMD_DELAY:
      delay(int(EEPROM.read(++i))); break;
    case CMD_DELAY_TEN:
      delay(10 * int(EEPROM.read(++i))); break;
    case CMD_BEGIN:
      break;
    case CMD_END:
      Keyboard.end();
      return;
    default:
      Keyboard.press(char(c));
      if (0 < click_length) {delay(click_length);}
      if (autorelease) {Keyboard.release(char(c));}
      break;
    }
  }
}

void high() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void low() {
  digitalWrite(LED_BUILTIN, LOW);
}

void blink(int i) {
    high();
    delay(i);
    low();
    delay(i);  
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    blink(250);
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  if (Serial.available() > 0) {
    blink(500);
    int b = Serial.read();
    switch(b) {
    case CMD_EXEC: execute(); break;
    case CMD_PROGRAM: program(); break;
    case CMD_DUMP: dump_eeprom(); break;
    /* case CMD_DUMP: dump_program(); break; */
    default: break;
    }
  }
  blink(1000);
  delay(100);
  Serial.write(int(CMD_NOOP));
}
