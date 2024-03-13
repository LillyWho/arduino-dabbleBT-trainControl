This project aims to make a simple 6V train control system that includes extra functions. In place of a gamepad, this uses the Dabble app, to emulate a controller with an analogue stick.
Connectivity is implemented via one of the compatible bluetooth chips, in my case the HC05.

Thanks to the L298N H-Bridge scaling the PWM output proportionally to the Arduino's 5V control signal, this can also be used with any power supply voltage that is electrically supported. However, when using a motor that is rated at a lower voltage than what is available to the h-bridge, make sure to adjust the maxSpeed variable to the percentage of the voltage that your motor supports.

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
- Cross: Emergency brake toggle. Sets speed to zero and blocks traction control until toggled again.

Parts list:
- Arduino knockoff with type C plug
- HC-05 bluetooth module
- A whole bunch of dupont jumper wires
- Smartphone
- L298N H-Bridge
- 6V LiPol rechargeable battery pack
- New Ray toy train
- 5V Ultrasonic Humidifier off ebay, credit to this video for the idea: https://www.youtube.com/watch?v=SeqZip3bmfQ
