# GameRing

NYUAD Design and Innovation 2020 Group 2


Game Ring

Objectives:

Create a device for entertainment purposes that makes use of a set of LEDs and an accelerometer/gyro sensor. The device can be used as the controlling element in a game or be a game in itself.

The following game/game modes were decided to meet the requirements:

1. _Catch the coin_

_This game mode makes use of the accelerometer and LEDs to make the player control a pixel that needs to be moved in order to catch a &quot;coin&quot; while avoiding an enemy. The player dies if they run into the enemy. The final score of the player is shown after they die. Shake the device to play again_

1. _Tap Delight_

_This game mode requires the user to tap the device when a moving pixel reaches a certain point in order to score. The player dies if they do not manage to stop it at the correct time twice in a row after which their final score is shown. Shake the device to play again._

1. _Spinner_

_The device acts as a conventional spinner that can be used in any sort of situation. Shake the device to spin the spinner again_

1. _Dice roll_

_The device lights up a random number of leds from 1 to 6 to simulate a dice. Shake the device to throw the dice again_

1. _Reaction game_

_This is a 2-player game, each player is assigned a colour and they have to tap the device as soon as their colour shows up on the ring. The winner is the user that has the lowest reaction time_

**Flipping the device upside down and flipping it back up again should change the game modes**

 Requirements

- The form factor that would be the most suitable for all the game modes above is a ring of pixels.
- 16 leds seems to be the right amount to suit all the games while preserving resources.
- The accelerometer/gyro module  (mpu-6050) needs to be in the center of the device to ensure accurate readings
- The device should be compact enough to fit into one hand to make is easy to play with
- The device should be able to lay flat on the table as well in case it is used as a spinner
- The LEDs should be clearly visible to the player
- The device should work off a AAA battery



Design

**PCB**

Please refer to the Bill of Materials file for a detailed list of the components used.

Going from the requirements, the pcb was made in a circular format following the schematics and the mpu-6050 module was placed in the exact middle of it. The PCB has a diameter of 70mm and 16 leds. The LEDs were placed equidistantly along the edge of a circle with the center at the middle of the pcb and of radius 30mm. The software used (EasyEDA) did not support automatic arrangement of components in a circle. So, a python script was written to calculate the coordinates and rotation values at which the LEDs should be placed on the PCB to ensure a perfect circle. The traces were mostly made using auto-routing features with some manual corrections. Mostly all components were placed on top of the board with the exception of the battery holder. 4 mounting holes were also placed on the PCB along the edge of a circle of radius 31mm .



**Enclosure**

The enclosure was designed with Fusion360. It has a bottom part on which the PCB is fixed an a threaded top lid that screws on the bottom part. The top part is to be translucent for the LEDs to shine through.


