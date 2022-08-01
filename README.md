# Science Subsystem

The science subsystem is responsible for digging, storing, testing, and monitoring dirt samples. The code in this repository accepts commands from the onboard computer and transmits data from its sensors back. Although there are commands to control individual motors and sensors, the value here is the `dig` command, which carries out all the steps for a successful test. 

This repository has two sketches: `Science` for the primary science board and `Science_Auger` for the auger control board. The sketches share a lot of standard code, which is refactored away into libraries in the `library` folder. 

## Setup

The Arduino IDE needs to know where those libraries are to compile the sketches. Unfortunately, Arduino will compile both sketches together if we place them in the same folder. The only other path Arduino recognizes is the user's sketchbook directory. Run the `sync-libraries.sh` script (or `sync-libraries.bat` for Windows) to sync the libraries here to your sketchbook.  

**Workaround** If the scripts do not work, manually copy the directories in `/libraries` into your sketchbook. With this approach, changes to the libraries won't synchronize with the sketchbook, and you'd need to re-copy the files every time. The script avoids this by linking the libraries. 
