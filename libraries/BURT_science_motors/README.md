# BURT_science_motors

Manages and controls the different motors used in the Binghamton University Rover Team's science subsystem.

The science subsystem has different types of motors, including linear stepper motors, rotating stepper motors, and DC motors. This library makes it simpler to manipulate these motors in ways useful for the science subsystem. This library offers four types of motors: 
- `LinearStepperMotor`: A stepper motor that moves in one dimension
- `RotatingStepperMotor`: A stepper motor that rotates along one axis
- `DCMotor`: A conventional motor that spins clockwise or counterclockwise.
- `Auger`: A specialized DC motor for the auger drill

Stepper motors can be moved in increments (millimeters or degrees) and cannot go beyond certain boundaries, denoted by limit switches. The DC motors can only spin at a given speed. Instances of this class (which correspond to the different physical motors) are declared in `BURT_science_pinouts.h`. To change pin numbers, go there. 

## Documentation
This library is documented using [Doxygen](https://doxygen.nl/)-style comments. This means that the `///` comments (specifically in the `.h` files) will provide more context and overview than regular comments. Doxygen can also generate neat web pages based on these comments. The documentation, however, is not checked into GitHub. To generate it:

- [Download](https://doxygen.nl/download.html) and install Doxygen 
- Open the Doxywizard program
- Click `File` > `Open` and select `Doxyfile.doxygen` in this folder
- Click `Run` > `Run Doxygen`. When finished, click `Show HTML Output`

## Usage

This library needs to be shared across two sketches, so we can’t just include it as part of either sketch. If we did, changes to one would not sync to the other. Instead, you have to make a link from your Arduino installation’s library folder. See the README for the `science` repository for details. 
