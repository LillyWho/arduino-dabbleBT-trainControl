#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

const int ledPin1 = 5; //pin D2, wire to GND
const int ledPin2 = 6;
/* CHANGEME: 
* I've limited the duty cycle in order to accomodate my use case of running a motor that can take a maximum of 4V, but if you intend to operate off a 12 or 24V power supply with a
* motor that can take 12 or 24V, then you can simply set this to 100 in order to allow for a full range duty cycle.
*/

const int maxSpeed = 72; 
const int Motor1 = 10;
const int in1 = 9;
const int in2 = 8;
const int puffPin = 4;
const int smokeMode = 0; // CHANGEME: 0 = diesel with closely pulsed smoke, 1 = steam with either constant in idle or puffs per cylinder beat in motion
static int mode = 0; // mode 0 = absolute analogue, mode 1 = real analogue throttle mode, mode 2 = d-pad incremental mode
static float directionPlus = 0;
static float directionMinus = 0;
static float direction = 0;
static int speed = 0;
static long interval = 0;
static long lastPuff = 0;
static long lastPuffOff = 0;
static bool doPuff = false;
static bool lightsOn = false;
static bool emergencyBrake = false;
static bool steamerEnabled = false;
static long lastTime = 0;
static long deltaTime = 0;

static int CircleDebounce = 0;
static int SquareDebounce = 0;
static int TriangleDebounce = 0;
static int XDebounce = 0;
static int StartDebounce = 0;
static long UpDebounce = 0;
static long DownDebounce = 0;
static long throttleTick = 0;


void setup() {

  pinMode(Motor1, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(ledPin1,OUTPUT);
  Serial.begin(9600);
  Dabble.begin(9600);
}

void loop() {
  Dabble.processInput();
  direction = GamePad.getYaxisData();
  directionPlus = constrain(direction,0.0,7);
  directionMinus = constrain(direction,-7.0,0);
  mode0(direction, mode == 0);
  mode1(mode == 1);
  mode2(mode == 2);
  diesel(steamerEnabled);
  digitalWrite(ledPin1,lightsOn);

  //Serial.print(interval);
  Serial.println(speed);
  //Serial.println(mode);
  digitalWrite(13,doPuff); //just represent it via the internal LED for now

  Circle(GamePad.isCirclePressed());
  Square(GamePad.isSquarePressed());
  Triangle(GamePad.isTrianglePressed());
  X(GamePad.isCrossPressed());
  Start(GamePad.isStartPressed());
  
}

void Circle(bool pressed) {
  if (!pressed) {CircleDebounce = 0; return 0;}
  if (CircleDebounce == 0) {CircleDebounce = millis(); lightsOn == !lightsOn;}
}
void Square(bool pressed) {
  if (!pressed) {SquareDebounce = 0; return 0;}
  if (SquareDebounce == 0) {SquareDebounce = millis(); modeDown();}
}
void Triangle(bool pressed) {
  if (!pressed) {TriangleDebounce = 0; return 0;}
  if (TriangleDebounce == 0) {TriangleDebounce = millis(); modeUp();}
}
void X(bool pressed) {
  if (!pressed) {XDebounce = 0; return 0;}
  if (XDebounce == 0) {XDebounce = millis(); emergencyBrake = !emergencyBrake;}
}
void Start(bool pressed) {
  if (!pressed) {StartDebounce = 0; return 0;}
  if (StartDebounce == 0) {StartDebounce = millis(); steamerEnabled = !steamerEnabled;}
}

void mode0(float value, bool mode0) {
  if (mode != 0) {return 0;}
  if (emergencyBrake) {speed = 0;analogWrite(Motor1, speed); return;}
  if (directionPlus > 0) {
    speed = lerp(value,0,7,0,maxSpeed); // map to the axis value if the speed isn't on emergency brake
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else {
    speed = lerp(value,0,7,0,maxSpeed); // map to the axis value if the speed isn't on emergency brake
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  analogWrite(Motor1, speed);
}
void mode1(bool mode1) {
  if (!mode1) {return 0;} //quit if we're not in mode 1
  if (emergencyBrake) {speed = 0;analogWrite(Motor1, speed); return 0;} //just brake if we hit the panic button
  if (speed == maxSpeed || speed == (maxSpeed * (-1))) {throttleTick = 0; analogWrite(Motor1, speed); return 0;} //reset tick variable, write to motor and exit
  if (speed > 0) {digitalWrite(in1,HIGH);digitalWrite(in2,LOW);} else if (speed < 0) {digitalWrite(in1,LOW); digitalWrite(in2,HIGH);}
  if (direction == 0) {
    throttleTick = 0;
    speed = speed + direction;
  } else if (direction != 0 && throttleTick == 0) {
    throttleTick = millis();
    speed = speed + direction;
  } else if (direction != 0 && millis() - throttleTick > 800) {
    throttleTick = millis();
    speed = speed + direction;
  }
  analogWrite(Motor1, abs(speed));
}
void mode2(bool mode2) {
  if (!mode2) {return 0;}
  if (emergencyBrake) {speed = 0; analogWrite(Motor1, abs(speed)); return;}
  if (GamePad.isUpPressed() && speed < maxSpeed) {
    if (UpDebounce == 0) {
      speed = constrain(speed + 2,maxSpeed * -1, maxSpeed);
      UpDebounce = millis();
    } else if (millis() - UpDebounce > 850) {
      speed = constrain(speed + 2,maxSpeed * -1, maxSpeed);
      UpDebounce = millis();
    }
  } else if (GamePad.isDownPressed() && speed > maxSpeed * -1) {
    if (DownDebounce == 0) {
      speed = constrain(speed - 2,maxSpeed * -1, maxSpeed);
      DownDebounce = millis();
    } else if (millis() - DownDebounce > 850) {
      speed = constrain(speed - 2,maxSpeed * -1, maxSpeed);
      DownDebounce = millis();
    }
  }
  if (!GamePad.isUpPressed()) {UpDebounce = 0;}
  if (!GamePad.isDownPressed()) {DownDebounce = 0;}
  analogWrite(Motor1, abs(speed));
  if (speed > 0) {digitalWrite(in1,HIGH);digitalWrite(in2,LOW);} else if (speed < 0) {digitalWrite(in1,LOW); digitalWrite(in2,HIGH);}
}
void modeUp() {
  if (mode == 2) return 0;
  mode = mode + 1;
}
void modeDown() {
  if (mode == 0) return 0;
  mode = mode - 1;
}
void steamer(bool steam) {
  //TODO: Steam engine timings
  if (!steam && smokeMode == 1) {doPuff = false; lastPuff = 0; lastPuffOff = 0; return 0;}
  if (!steam && smokeMode != 1) {return 0;}
///////////////////////////////////////////////////////////////////////////
  if (!doPuff && millis() - lastPuffOff > 5000) {
    doPuff = true;
    lastPuff = millis();
  }
  if (doPuff && millis() - lastPuff > 2000) {
    doPuff = false;
    lastPuffOff = millis();
  }
}
void diesel(bool steam) {
  if (!steam && smokeMode == 0) {doPuff = false; lastPuff = 0; lastPuffOff = 0; return 0;}
  if (!steam && smokeMode != 0) {return 0;}
  interval = lerp(abs(speed),0,maxSpeed,20,180);
  if (speed == 0) { //idle smoke behaviour
    if (!doPuff && millis() - lastPuffOff > 250) {
    doPuff = true;
    lastPuff = millis();
    }
    if (doPuff && millis() - lastPuff > 250) {
      doPuff = false;
      lastPuffOff = millis();
    }
  } else if (abs(speed) > 0) {
    if (!doPuff && millis() - lastPuffOff > 250 + (interval * -1)) {
      doPuff = true;
      lastPuff = millis();
    }
    if (doPuff && millis() - lastPuff > 250 + (interval * -1)) {
      doPuff = false;
      lastPuffOff = millis();
    }
  }
}

float lerp(float x,float x0, float x1, float y0, float y1) {
    float d = x1 - x0;
    if (d == 0) {
        return (y0 + y1) / 2;
    }
    return y0 + (x - x0) * (y1 - y0) / d;
}
int abs(int x) {
    return (x < 0) ? -x : x;
}
