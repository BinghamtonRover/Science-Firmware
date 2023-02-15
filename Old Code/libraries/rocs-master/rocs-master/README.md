# Installation
This procedure only needs to be performed initially or whenever there is an update to rocs.
1. Either `git clone` or download and extract a zip file of this repository to `<your arduino sketch dir>/libraries/rocs`. On my machine, for example, the arduino sketch dir is `~/Arduino`, so I would put this repository in `~/Arduino/libraries/rocs/`, and I would expect to see files like `~/Arduino/libraries/rocs/rocs.cpp`, etc.
2. Open or restart the Arduino IDE. Confirm that rocs was installed correctly by looking at the `File > Examples` menu: you should see a `rocs` submenu listed under `Examples from Custom Libraries`.

# Use
This procedure only needs to be performed once per sketch.
1. Once the library is installed, it can be included in the current sketch by navigating to the `Sketch > Include Library` menu and selecting `rocs` from the list under `Contributed libraries`.
2. Make sure to add `#include <rocs.hpp>` to the top of your sketch code in order to use the library.
3. See the bundled example (can be found in the Arduino examples menu as explained in the Installation section) for usage instructions.

# The Concept
ROCS is a super-lightweight protocol that sits atop I2C. It is barely a protocol at all; its purpose is to provide a simple, easy-to-understand-and-use abstraction over I2C such that low-level communication details are of no concern to the programmer. A ROCS network consists of a single **Controller** and multiple **Responders**. Only the Controller may initiate communication at any time. A given Responder will only send or receive information at the Controller's request. This model is well-suited to the rover; the central computer is assigned as the Controller and all subsystems are Responders.

Each Responder provides 256 "virtual registers". These are one-byte "memory cells" which can be read from and written to. Register 0 is always controlled by ROCS; it reports the name of the Responder to which that register belongs. All other registers (ids 1 - 255, inclusive) can be utilized by the user. Since the only valid register operations are read and write, ROCS consists of two types of packets: read and write.

A ROCS read request, issued by the Controller, will specify the I2C address of a Responder and the register from which to read. The Responder at that address, if any exists, will respond to this request with a single byte (the "contents" of the requested register). The semantic meaning of the request register and the returned byte is up to the user.

A ROCS write request, issued by the Controller, will specify the I2C address of a Responder, a register, and a byte which will be written to the specified register. The Responder at that address, if any exists, will receive the data byte and process it. The semantic meaning of the request register and data byte is up to the user.

ROCS does not handle Responder discovery; it is recommended that addresses be hardcoded (since they need to be unique anyway). Due to the synchronous nature of I2C, a read from/write to an address without a Responder will fail after a (short) amount of time, so it is easy to determine if a Responder doesn't exist (or is not responding properly).

# Packet Format
The following descriptions are from the perspective of the Controller.
## Read
A ROCS read is performed by initiating a one-byte WRITE over I2C to a Responder, sending the one-byte id of the requested register. Then, a one-byte READ is requested from that Responder, which is interpreted to contain the value of the requested register.
## Write
A ROCS write is performed by initiating a two-byte WRITE over I2C to a Responder, sending the one-byte id of the requested register followed immediately by the new value of that register. Note that both bytes are sent in a single I2C WRITE.
## Register 0
A read from a Responder's register 0 always returns the name of that Responder. The first read from reg 0 provides the length `n` of the name string. Each following `i`th read will provide the `i`th character of the name, until `n` total reads are accomplished (not including the initial length read).

Example:
```
Controller READ FROM Responder(id 0x11, reg 0) = 7

// 7 is the length of the Responder's name.
// Must read each character. 7 total reads:

Controller READ FROM Responder(id 0x11, reg 0) = 'E'
Controller READ FROM Responder(id 0x11, reg 0) = 'x'
Controller READ FROM Responder(id 0x11, reg 0) = 'a'
Controller READ FROM Responder(id 0x11, reg 0) = 'm'
Controller READ FROM Responder(id 0x11, reg 0) = 'p'
Controller READ FROM Responder(id 0x11, reg 0) = 'l'
Controller READ FROM Responder(id 0x11, reg 0) = 'e'

// At this point, the next read would return 7 again.
```

Note that it is the Controller's responsibility to read the entire name; otherwise, one would read from register 0 expecting the name length, get a character instead, interpret that character as the length, and read a bogus string.

# Where is the code?
This repository contains Responder-side Arduino code.
The Controller-side code lives in the RoverSystem repository, under `src/rocs`.

# What do I do with this information?
Generally, one should pretend registers are "remote" variables that can be written to and read from just like normal variables. For example, the drive control Responder might provide 4 registers:
| Register Id | Register Name | Register Behavior |
| - | - | - |
| 1 | Left Direction | Direction of the left-side wheels. 0 is forward and 1 is reverse. |
| 2 | Right Direction | Direction of the right-side wheels. 0 is forward and 1 is reverse. |
| 3 | Left Speed | Speed of the left-side wheels. 0 is stopped. 1 - 255 scales wheel rotation such that 255 is max speed. |
| 3 | Right Speed | Speed of the right-side wheels. 0 is stopped. 1 - 255 scales wheel rotation such that 255 is max speed. |

Of course, there are multiple ways of doing something like this. An alternative might be:
| Register Id | Register Name | Register Behavior |
| - | - | - |
| 1 | Left | Direction and speed of left-side wheels. Interpreted as a signed 2's complement 8-bit integer. 0 is stopped. -128 is max speed in reverse, 127 is max speed forward, and everything in between scales accordingly. |
| 2 | Right | Direction and speed of right-side wheels. Interpreted as a signed 2's complement 8-bit integer. 0 is stopped. -128 is max speed in reverse, 127 is max speed forward, and everything in between scales accordingly. |

While a register's content is described in this document as an unsigned 8-bit integer, those bits can be interpreted however one chooses. The second example above, for instance, chooses to interpret register contents as a signed integer, which reduces the number of registers required to control the suspension at the expense of less granularity over speed control.

In general, one should use an additional register over getting fancy with the contents of an existing register (such as the second example above, or the register 0 behavior). This ensures simplicity and preserves the "remote variable" illusion. A complicated Responder (such as the arm controller) might require a large amount of functionality, but it is unlikely that such functionality would require 255 registers. For instance, the arm might keep note of its orientation in space after homing. Say that the arm has 6 DOF. Then there could be 6 registers, one for each axis of movement or rotation, and a relative amount to move could be written to an axis's register. Extra status info (such as, is this axis still? Is this axis moving?) could be provided, which would account for another 6 registers. Then, axis speed could be set, which would include another 6 registers. Oh, and direction could be conrolled by a separate register to allow for finer distance granularity, so add another 6 to the list. All of this only equals 24 registers, which is only about 9.4% of all ROCS registers. This Responder could control 10 arms and still have registers left over! Don't be afraid of using more registers!

Storing as much state as possible on the Responder side will limit the amount of information that must be passed between Responder and Controller, which will naturally limit the number of registers required.

If a large amount of data must be transferred, don't use ROCS.

# Help! I'm not getting a response / I'm getting bogus values!
It's probably a hardware issue. Make sure communication wires are relatively short and don't pass too close / too parallel to high-current lines (like motor or power wires). Using shielded cable is strongly recommended. https://hackaday.com/2017/02/08/taking-the-leap-off-board-an-introduction-to-i2c-over-long-wires/ contains information about dealing with long-distance I2C issues. Note that boards such as the Nvidia TX2/Nano and Raspberry Pi often have built-in pullup resistors on their I2C lines, so take that into consideration if pursuing a better pullup resistor value.
