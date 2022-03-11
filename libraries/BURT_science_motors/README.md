# BURT_science_motors

Manages and controls the different motors used in the Binghamton University Rover Team's science subsystem.

The science subsystem has different types of motors, including linear stepper motors, rotating stepper motors, and DC motors. This library makes it simpler to manipulate these motors in ways useful for the science subsystem.  

For now, these libraries have to be synced with the global Arduino sketchbook to be included in both `Science.ino` and `Science_Auger.ino`. Once the two sketches are merged, place this library in `Science/src` to make it self-contained within the sketch.
