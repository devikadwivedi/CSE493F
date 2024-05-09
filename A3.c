A#include <ParallaxJoystick.hpp>
#include <Mouse.h>
#include <Keyboard.h>

const int JOYSTICK_UPDOWN_PIN = A1;
const int JOYSTICK_LEFTRIGHT_PIN = A0;
const int MAX_ANALOG_VAL = 1023;
const int ANALOG_CENTER_VALUE = int(MAX_ANALOG_VAL / 2);
const enum JoystickYDirection JOYSTICK_Y_DIR = RIGHT;
const int MOUSE_MOVEMENT_THRESHOLD = 10;
const int MAX_MOUSE_MOVE_VAL = 10;
const int BUTTON_MOUSE_CLICK_PIN = 2;
const int BUTTON_DELETE_PIN = 4;
const int BUTTON_SPACE_PIN = 7;
ParallaxJoystick _analogJoystick(JOYSTICK_UPDOWN_PIN, JOYSTICK_LEFTRIGHT_PIN, MAX_ANALOG_VAL, JOYSTICK_Y_DIR);

int prevButtonDeleteVal = HIGH;
int prevButtonSpaceVal = HIGH;
int prevButtonMouseClickVal = HIGH;

const int VIBROMOTOR_OUTPUT_PIN = 5;
const int POT_PIN = A2;
/*
 * Sources: I collaborated with Ritesh Kanchi on this code. I used chatgpt for a guide to map the joystick values to the mouse position on screen.
 * I'm using bits and pieces of different code provided by the course examples and my previous projects.
 */
void setup() {
  Keyboard.begin();
  pinMode(BUTTON_MOUSE_CLICK_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DELETE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SPACE_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  Mouse.begin();
}

void loop() {
  int analogX = analogRead(JOYSTICK_LEFTRIGHT_PIN);
  int analogY = analogRead(JOYSTICK_UPDOWN_PIN);
  int xDistFromCenter = analogX - ANALOG_CENTER_VALUE;
  int yDistFromCenter = analogY - ANALOG_CENTER_VALUE;
  int yMouse = 0, xMouse = 0;
  if (abs(xDistFromCenter) > MOUSE_MOVEMENT_THRESHOLD) {
    xMouse = map(analogX, 0, MAX_ANALOG_VAL, -MAX_MOUSE_MOVE_VAL, MAX_MOUSE_MOVE_VAL);
  }
  if (abs(yDistFromCenter) > MOUSE_MOVEMENT_THRESHOLD) {
    yMouse = map(analogY, 0, MAX_ANALOG_VAL, -MAX_MOUSE_MOVE_VAL, MAX_MOUSE_MOVE_VAL);
  }
  int mouseClickBtn = digitalRead(BUTTON_MOUSE_CLICK_PIN);
  int buttonDeleteVal = digitalRead(BUTTON_DELETE_PIN);
  int buttonSpaceVal = digitalRead(BUTTON_SPACE_PIN);
  Mouse.move(xMouse, yMouse, 0);

  if (!Mouse.isPressed() && mouseClickBtn == LOW) {
    Mouse.press();
    vibrate(100);
  } else if (Mouse.isPressed() && mouseClickBtn == HIGH) {
    Mouse.release();
  }

  if (prevButtonDeleteVal != buttonDeleteVal && buttonDeleteVal == LOW) {
    Keyboard.press(KEY_DELETE);
    vibrate(100);
  } else if (prevButtonDeleteVal == LOW && buttonDeleteVal == HIGH) {
    Keyboard.release(KEY_DELETE);
  }
  prevButtonDeleteVal = buttonDeleteVal;

  if (prevButtonSpaceVal != buttonSpaceVal && buttonSpaceVal == LOW) {
    Keyboard.press(' ');
    vibrate(100);
  } else if (prevButtonSpaceVal == LOW && buttonSpaceVal == HIGH) {
    Keyboard.release(' ');
  }
  prevButtonSpaceVal = buttonSpaceVal;
  delay(10);
}
