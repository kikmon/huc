# example programs

This directory contains a number of example programs and utilities for developing on the PC Engine in assembly-language.


## Utilities

* ipl-scd
  - A customized CD-ROM IPL that either displays an error message or runs an error overlay, when not booted on a Super CD-ROM.

* ted2-bios-romcd
  - A modified System Card 3.0 HuCARD that runs a CD-ROM overlay stored inside the HuCARD image, rather than loading it from CD-ROM.

* ted2-bios-usbcd
  - A modified System Card 3.0 HuCARD that runs a CD-ROM overlay uploaded through USB, rather than loading it from CD-ROM.

* rom-bare-vdctest
  - A HuCARD ROM to compare a PC Engine emulator's VDC RCR interrupt handling with real PC Engine console hardware.


## Using the System Card BIOS

* scd-bios-hello
  - A simple example of creating an ISO for a Super CD-ROM, and using the System Card BIOS.

* scd-bios-hello-error
  - An expansion of scd-bios-hello, using an error overlay to show a pretty screen when run with System Card 2.0 or earlier.


## Getting started with the "CORE(not TM)" PC Engine library

The "CORE(not TM)" PC Engine library is a small and configurable set of library routines, using less than 400 bytes at a minimum, that sets up a consistent PC Engine environment for both HuCARD and CD-ROM, and which provides interrupt handling, joypad reading, and file loading (on CD-ROM).


* rom-core-hello
  - A simple example of creating a HuCARD ROM.

* cd-core-1stage
  - A simple example of creating an ISO for a CD-ROM, and loading the "CORE(not TM)" kernel code from the overlay program.

* cd-core-2stage
  - A simple example of creating an ISO for a CD-ROM, and loading the "CORE(not TM)" kernel code in a startup overlay.

* scd-core-1stage
  - A simple example of creating an ISO for a Super CD-ROM, and loading the "CORE(not TM)" kernel code from the overlay program.

* scd-core-2stage
  - A simple example of creating an ISO for a Super CD-ROM, and loading the "CORE(not TM)" kernel code in a startup overlay.

* scd-core-1stage-error
  - An expansion of scd-core-1stage, using an error overlay to show a pretty screen when run with System Card 2.0 or earlier.

* scd-core-2stage-error
  - An expansion of scd-core-2stage, using an error overlay to show a pretty screen when run with System Card 2.0 or earlier.

* ted2-core-hwdetect
  - An example of detecting PC Engine hardware and testing the Joypads and Mice attached to the console.


## Getting started with the KickC compiler

* rom-kickc-hello
  - A simple example of creating a HuCARD ROM.

* rom-kickc-shmup
  - A simple conversion of HuC's shmup.c sample to KickC.
