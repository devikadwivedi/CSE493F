//
const int LED_OUTPUT_PIN = 3;
const int RED_LED_PIN = 11;
const int GREEN_LED_PIN = 10;
const int BLUE_LED_PIN = 9;
const int PHOTOCELL_INPUT_PIN = A0;
const int BUTTON_INPUT_PIN = 2;
const int MIN_PHOTOCELL_VAL = 200; 
const int MAX_PHOTOCELL_VAL = 800; 
const boolean PHOTOCELL_IS_R2_IN_VOLTAGE_DIVIDER = true;
int buttonState = 0;
int lastButtonState = 0;
int currentMode = 2;
const int MAX_COLOR_VALUE = 255;
enum RGB{
  RED,
  GREEN,
  BLUE,
  NUM_COLORS
};

int _rgbLedValues[] = {255, 0, 0}; // Red, Green, Blue
enum RGB _curFadingUpColor = GREEN;
enum RGB _curFadingDownColor = RED;
const int FADE_STEP = 5;

void setup() {
  pinMode(LED_OUTPUT_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(PHOTOCELL_INPUT_PIN, INPUT);
  pinMode(BUTTON_INPUT_PIN, INPUT);
}

void loop() {
  // The code in this loop is largely pasted from ChatGPT
  buttonState = digitalRead(BUTTON_INPUT_PIN);
  if (buttonState == HIGH && lastButtonState == LOW) {
    currentMode = (currentMode + 1) % 3;
  }
  lastButtonState = buttonState;

  switch (currentMode) {
    case 0:
      fsrMode();
      break;
    case 1:
      potMode();
      break;
    case 2:
      gradientMode();
      break;
  }
  delay(100);
}

void fsrMode() {
  int fsrValue = analogRead(A3);
  //Serial.println(fsrValue);
  int brightness = map(fsrValue, 0, 500, 0, 255);
  analogWrite(RED_LED_PIN, 255 - brightness);
  analogWrite(GREEN_LED_PIN, 0);
  analogWrite(BLUE_LED_PIN, 255 - brightness);
}

void potMode() {
  int potVal = analogRead(A2);
  //Serial.println(potVal);
  int greenVal = map(potVal, 300, 570, 50, 20);
  analogWrite(RED_LED_PIN, 190);
  analogWrite(GREEN_LED_PIN, greenVal);
  analogWrite(BLUE_LED_PIN, 0);
  delay(400);
}


// The following method is from the physical computing textbook:
// https://makeabilitylab.github.io/physcomp/sensors/photoresistors.html
int findLed() {
  int photocellVal = analogRead(PHOTOCELL_INPUT_PIN);
  int ledVal = map(photocellVal, MIN_PHOTOCELL_VAL, MAX_PHOTOCELL_VAL, 0, 255);
  ledVal = constrain(ledVal, 0, 255);
  
  if(PHOTOCELL_IS_R2_IN_VOLTAGE_DIVIDER == false){
    ledVal = 255 - ledVal;
  }
  return ledVal;
}

// The following method is from the physical computing textbook:
// https://makeabilitylab.github.io/physcomp/arduino/rgb-led-fade.html
void gradientMode() {
  int ledVal = findLed();
  _rgbLedValues[_curFadingUpColor] += FADE_STEP;
  _rgbLedValues[_curFadingDownColor] -= FADE_STEP;
      
  if(_rgbLedValues[_curFadingUpColor] > MAX_COLOR_VALUE){
    _rgbLedValues[_curFadingUpColor] = MAX_COLOR_VALUE;
    _curFadingUpColor = (RGB)((int)_curFadingUpColor + 1);

    if(_curFadingUpColor > (int)BLUE){
      _curFadingUpColor = RED;
    }
  }

  if(_rgbLedValues[_curFadingDownColor] < 0){
    _rgbLedValues[_curFadingDownColor] = 0;
    _curFadingDownColor = (RGB)((int)_curFadingDownColor + 1);

    if(_curFadingDownColor > (int)BLUE){
      _curFadingDownColor = RED;
    }
  }
  setColor(_rgbLedValues[RED], _rgbLedValues[GREEN], _rgbLedValues[BLUE], ledVal);
}

void setColor(int red, int green, int blue, int ledVal) {
  float val = (255.0 - ledVal) / 255.0;
  analogWrite(RED_LED_PIN, (MAX_COLOR_VALUE - red) * val);
  analogWrite(GREEN_LED_PIN, (MAX_COLOR_VALUE - green) * val);
  analogWrite(BLUE_LED_PIN, (MAX_COLOR_VALUE - blue) * val);
}
