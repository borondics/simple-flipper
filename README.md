# simple-flipper
Arduino-based controller for beam shutter and flip mirror applications

The repository provides open-source hardware and software designs for very simple servo based applications requiring the switching between two predifined positions. Exxamples: beam shutter, flip mirror, etc.

# :warning: :warning: :warning: Disclaimer :bangbang: :bangbang: :bangbang:

This project is far from finished, the current state only allows limited functionality and not proprely tested in all experimental situations. Use it with caution, and if you have suggestions or want to join development please contact us, make your own fork, create pull requests, submit issues, leave comments ....  

# Components

## Electronics

The electronic design is based on an [Arduino nano](https://docs.arduino.cc/hardware/nano/) (but any type can be used) and [PCA9685 16-Channel Servo Driver](https://learn.adafruit.com/16-channel-pwm-servo-driver/).

The arduino code provided in the .code folder gives a very basic control implementation with three predefined positions (servo pwm value has to be calibrated before used, [see](https://learn.adafruit.com/16-channel-pwm-servo-driver/using-the-adafruit-library))

The arduino communicates to the board by I2C thus only a few connection are required. 
The software implementation is using the PCA9685 16-Channel PWM Driver Module Library library.
### Wiring

The following wiring is adviced especially with using multiple servos:
![basic_wiring](/images/basic_wiring_image.png)

For further infromation we refer to the servo board documentation.

Our implementation looks like this:
PHOTO
<!-- ![cad_design](/images/cad_design.png) -->

### Code and communication

The arduino code handles the servo control (position values: SERVOMIN, SERVOMAX, SERVOMID have to be set) and provides a very basic command set to change positions and get the current position values using serial commands (using [Arduino-CommandParser](https://docs.arduino.cc/libraries/commandparser/)).

Currently, the following protocol was implemented:

Command: SETPOS A motornumber
Action: *Moves the servo addresse by <int: motornumber> to position A*
Example: "POS A 0"

Command: SETPOS B motornumber
Action: *Moves the servo addresse by <int: motornumber> to position B*
Example: "POS B 0"

Command: SETPOS O motornumber
Action: Moves the servo addresse by <int: motornumber> to home position with is the middlepoint between A and B

Command: GETPOS motornumber
Action: Returns the current position of the mirror flip based on the PWM value of the controller board


## License

This work is licensed under a
[Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg
