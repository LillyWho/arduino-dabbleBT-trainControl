This project aims to make a simple 5V train control system that includes extra functions. In place of a gamepad, this uses the Dabble app, to emulate a controller with an analogue stick.
Connectivity is implemented via one of the compatible bluetooth chips, in my case the HC05.

Currently implemented:

- Different control modes, 0 for absolute analogue control forwards and backwards, 1 for throttle-based control, 2 for d-pad control
- LED for headlights on/off
- Ultrasonic humidifier on/off for steam output, planned to be dynamically responsive to the loco's traction. Includes a mode for diesel, and a mode for steam exhaust dynamics.

Controls:
- START: Smoke on/off
- Triangle: Traction Control Mode Up
- Square: Traction Control Mode Down
- Circle: Headlights
- Traction control for various modes should be self-explanatory
