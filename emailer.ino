#include <Keyboard.h>

// Random constants

int TYPING_DELAY = 20;
int DELAY_MULTIPLIER = 2; // for testing
void _delay(int i) { delay(DELAY_MULTIPLIER * i); }

int OS = -1, MAC = 0, WIN = 1, LNX = 2;
char CMD_KEYS[3] = {char(KEY_LEFT_GUI), char(KEY_LEFT_CTRL), char(KEY_LEFT_CTRL)};

// Keyboard helper stuff

void press_and_release(String s) {
  for (int i = 0; i < s.length(); i++) {Keyboard.press(s.charAt(i));}
  delay(TYPING_DELAY);
  for (int i = 0; i < s.length(); i++) {Keyboard.release(s.charAt(i));}
}
void press_and_release(int i) { press_and_release(String(char(i))); }
void tab() { press_and_release(KEY_TAB); }
void enter() { press_and_release(KEY_RETURN); }
void cmd(char c) { press_and_release(String(CMD_KEYS[OS]) + c); }
void type(String s) {
  for (int i=0; i < s.length(); i++){
    press_and_release(s.charAt(i));
  } 
}

// Logic

void send_email(String recipient, String subject, String contents) {
  if (MAC == OS) {
    // Open email client (outlook, in this case)
    cmd(' '); _delay(100); // open spotlight
    type("outloo"); enter(); _delay(250); // search for outlook

    // Send the message
    cmd('n'); _delay(400); // open a new message
    type(recipient); _delay(300); // enter the recipient string
    tab(); enter(); _delay(300); // tab down to the cc line and then the subject line
    // I've encountered at least one computer that requires an extra tab or enter here to get past the bcc field.
    // I don't know of a good way to handle this.
    // If you add an extra enter, you run the risk of putting the subject line in the body of the message.
    type(subject); _delay(300); // enter the subject
    tab(); _delay(400); // tab down to the message body    
    type(contents); enter(); // enter the message contents
    //cmd(KEY_RETURN); // send - Commented out to make it safe to test; uncomment this command to weaponize this.
    //enter(); // If you add the extra enter above that risks leaving an empty subject line,
    // you need an extra enter here to accept the "are you sure you want to send this without a subject" warning prompt.
  }
}

void setup() {
  // Eventual plan is to have the OS be determined based on which button of the USB hub is depressed.
  // The board I'm using should just barely be able to fit inside a Sabrent 4-port USB hub without removing any guts.
  // One of the ports will be swapped and used for the board.
  // The other three will be active but will have the light leads for the keys (or the power leads for the ports, not sure) tapped to use as signals to the arduino.
  // One button for Mac, one button for Windows, one button for Linux, and one on/off button for the arduino itself.
  // By running at startup, it's easy to re-send the message by powering it down and then back up.
  OS = MAC;
  Keyboard.begin();
  _delay(500);
  send_email(
    "your_department@HipstrStartup.co",
    "I love donuts!!1!",
    "Dear everybody,\n\nI love donuts, so I'll bring some to share tomorrow.\n\n-Someone who forgot to lock their computer"
  );
}

void loop() {}
