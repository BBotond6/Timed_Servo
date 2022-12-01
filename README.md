# Timed_Servo
This Arduino project was created to control with push buttons the periodic operation of a servo motor.

## Used Components
- Arduino UNO R3
- 9G Micro Servo
- 7-segment display
- 2 Push buttons
- 2 LED
- 5 Resistors

## Schematic Drawing
![shematic](https://user-images.githubusercontent.com/102668658/204998077-9d45becb-1871-4468-af0a-4a122ce74264.png)

## Working principle
- One button controls switching the servo motor on and off.
- The others button steps the value of the 7-segment display from 1 to 9.
- The number displayed on the 7-segment display is in minutes and it will be the timing of the periodic movement.
- The blue LED lights up when the servo motor is switched on.
- 10 seconds before the movement, the blue LED starts blinking.
