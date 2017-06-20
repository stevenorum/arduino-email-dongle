#include <Keyboard.h>

void pressAndRelease(char c) {
  Keyboard.press(c);
  Keyboard.release(c);
}

void tab() { pressAndRelease(KEY_TAB); }

void enter() { pressAndRelease(KEY_RETURN); }

void cmd(char c) {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(c);
  Keyboard.releaseAll();
}

void type(String s) { for (int i=0; i < s.length(); i++){ pressAndRelease(s.charAt(i)); } }

void open_outlook() {
  cmd(' ');
  delay(100);
  type("outloo");
  enter();
  delay(250);
}

void email(String recipient, String subject, String contents) {
  cmd('n');
  delay(400);
  type(recipient);
  delay(300);
  tab();
  enter();
  delay(300);
  type(subject);
  delay(300);
  tab();
  delay(400);
  type(contents);
  enter();
  cmd(KEY_RETURN);
}

void setup() {
  Keyboard.begin();
  delay(500);
  open_outlook();
  email(
    "your_department@HipstrStartup.co",
    "I love donuts!!1!",
    "Dear everybody,\n\nI love donuts, so I'll bring some to share tomorrow.\n\n-Someone who forgot to lock their computer"
  );
}

void loop() {}
