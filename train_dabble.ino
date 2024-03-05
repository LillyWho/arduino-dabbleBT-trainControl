#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

const int ledPin1 = 5; //pin D2, wire to GND
const int maxSpeed = 72;
const int Motor1 = 10;
const int in1 = 9;
const int in2 = 8;
const int puffPin = 5;
const int smokeMode = 0; // CHANGEME: 0 = diesel with closely pulsed smoke, 1 = steam with either constant in idle or puffs per cylinder beat in motion
static int mode = 0; // mode 0 = absolute analogue, mode 1 = real analogue throttle mode, mode 2 = d-pad incremental mode
static float directionPlus = 0;
static float directionMinus = 0;
static float direction = 0;
static float speed = 0;
static long lastPuff = 0;
static long lastPuffOff = 0;
static bool doPuff = false;
static bool lightsOn = false;
static bool emergencyBrake = false;
static bool steamerEnabled = false;
void setup() {

  pinMode(Motor1, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(ledPin1,OUTPUT);
  Serial.begin(115200);
  Dabble.begin(9600);
}

void loop() {
  Dabble.processInput();
  direction = GamePad.getYaxisData();
  directionPlus = constrain(direction,0.0,7);
  directionMinus = constrain(direction,-7.0,0);
  speed = constrain(speed,0,maxSpeed);
  modeUp(GamePad.isTrianglePressed());
  modeDown(GamePad.isSquarePressed());
  steamer(steamerEnabled);
  if (GamePad.isCrossPressed())
  {
    emergencyBrake = !emergencyBrake; // toggle emergency brake
  }

  if (GamePad.isCirclePressed())
  {
    lightsOn = !lightsOn;
  }
  if (GamePad.isStartPressed()) {
    steamerEnabled = !steamerEnabled;
  }

  if (lightsOn) {
    digitalWrite(ledPin1,ON);
  } else {
    digitalWrite(ledPin1,OFF);
  }
  if (lightsOn) {
    digitalWrite(13,1);
  } else {
    digitalWrite(13,0);
  }
  
}
void mode0(float value) {
  if (!emergencyBrake) {
    if (directionPlus > 0) {
      speed = map(value,0, 1.0, 0 , maxSpeed); // map to the axis value if the speed isn't on emergency brake
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    }
    else {
      speed = map(value,-1, 0, 0 , maxSpeed); // map to the axis value if the speed isn't on emergency brake
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
  } else {
    speed = 0;
  }
  analogWrite(Motor1, speed);
}
void mode1() {
  speed = speed + direction;
  analogWrite(Motor1, speed);
}
void mode2() {
  if (GamePad.isUpPressed()) {
    speed = speed + 2;
  } else if (GamePad.isDownPressed()) {
    speed = speed - 2;
  }
  analogWrite(Motor1, speed);
}
void modeUp(bool pressed) {
  if (!pressed) return 0;
  if (mode == 2) return 0;
  mode = mode + 1;
}
void modeDown(bool pressed) {
  if (!pressed) return 0;
  if (mode == 0) return 0;
  mode = mode - 1;
}
void steamer(bool steam) {
  if (!steam) {doPuff = false; lastPuff = 0; return 0;}
  if (lastPuff == 0 || millis() - lastPuffOff > 200) {
    doPuff = true;
    lastPuff = millis();
    lastPuffOff = 0;
  }
  if (lastPuff != 0 && millis() - lastPuff > 200) {
    doPuff = false;
    lastPuff = 0;
    lastPuffOff = millis();
  }
}