; ***************************************************************************
; ***************************************************************************
;
; core-config.inc
;
; Configuration settings for the "CORE(not TM)" PC Engine library code.
;
; Copyright John Brandwood 2021.
;
; Distributed under the Boost Software License, Version 1.0.
; (See accompanying file LICENSE_1_0.txt or copy at
;  http://www.boost.org/LICENSE_1_0.txt)
;
; ***************************************************************************
; ***************************************************************************
;
; The idea is that you, a PCE developer, copy this file from the ../include/
; directory and into your project's directory, and then customize the values
; to suit your particular project.
;
; Because PCEAS searches the current (i.e. project) directory for an include
; file first, then it will find your customized copy of the file rather than
; the original copy in the ../include/ directory.
;
; That means that all of the different overlay programs in your project will
; share the same overall library configuration for your game.
;
; ***************************************************************************
; ***************************************************************************

	.ifndef	USING_MPR7
;
; Are we going to take over MPR7?
;
; While this is the default for the "CORE(not TM)" library, it is definitely
; possible to use the library to build CD-ROM code that runs with the System
; Card in MPR7, and with the 1st bank remaining in MPR2.
;
; N.B. The CD-ROM Stage1 loader runs the library with USING_MPR7=0 while it
;      installs the kernel into PCE RAM in MPR1.
;

USING_MPR7	=	1	; (0 or 1)
	.endif

	.ifndef	USING_STAGE1
;
; Are we building a CD-ROM overlay that uses an already-built CD-ROM Stage1
; loader to install the library's kernel code into PCE RAM in MPR1?
;
; If this is set, the library's kernel code will be excluded from the build,
; which will save about 1KB-3KB of code space in every overlay program.
;
; N.B. The CD-ROM Stage1 loader sets USING_STAGE1=0 so it builds correctly.
;

USING_STAGE1	=	0	; (0 or 1)
	.endif

;
; Is the last track of the CD a duplicate of the main ISO data track?
;
; Most CD games put two copies of the same ISO track on the CD, so that data
; can be read from the secondary track if the drive has a problem reading it
; it from the primary location.
;
; If set, the secondary track will be enabled when the library 1st starts.
;

SUPPORT_2ISO	=	0	; (0 or 1)

;
; Maximum number of directory entries to use from the ISO.
;
; The fewer that we choose, the less RAM memory we need. A setting of 64 is
; a good compromise to use until you are putting lots of files on the ISO.
;

MAX_DIRSIZE	=	64	; (4..256)

;
; Use the System Card's PSG driver code instead of a modern alternative
; sound driver like a DefleMask player?
;

USING_PSGDRIVER	=	0	; (0 or 1)

;
; Support development for the Turbo EverDrive v2?
;
; This only applies to HuCard programs, but when chosen, the "CORE(not TM)"
; kernel is moved from bank 0 to bank 1, and a minimal set of startup code
; is put in bank 0.
;
; This is because the TED2 hardware occupies the whole of bank 0 when it is
; activated, and so we need to run with a different bank in MPR7.
;

SUPPORT_TED2	=	0	; (0 or 1)

;
; Support development for the SuperGrafx?
;
; This enables SGX hardware support in certain library functions.
;

SUPPORT_SGX	=	0	; (0 or 1)

;
; Select which version of the joystick library code to include, only one of
; these can be set to '1' ...
;
; SUPPORT_2BUTTON : Only returns buttons I and II.
; SUPPORT_6BUTTON : Read buttons III-VI, but ignore a mouse.
; SUPPORT_MOUSE	  : Read mouse, but ignore buttons III-VI.
;
; It doesn't make sense to design a game the relies on both the 6-button and
; the mouse, so the joystick library is optimized for one or the other.
;
; Note that both those devices are always detected and no conflicts occur,
; this just controls reading either buttons III-VI or the Mouse Y-movement.
;

SUPPORT_2BUTTON	=	1	; (0 or 1)
SUPPORT_6BUTTON	=	0	; (0 or 1)
SUPPORT_MOUSE	=	0	; (0 or 1)

;
; How many joypad/mouse devices should be supported?
;
; This is normally 5, but can be set to 3 (or lower) in order to speed up
; the processing and free up CPU time for other code, which is especially
; useful for mouse games.
;

MAX_PADS	=	5	; (1..5)

;
; Control the "slow" and "fast" delays for auto-repeating the UP and DOWN
; button on the joypads.
;
; The "slow" setting is the delay from when the direction is held to the
; first auto-repeat.
;
; The "fast" setting is the delay from when the first auto-repeat to all
; subsequent auto-repeats until the direction is released. 
;
; Set to "0" to disable auto-repeat.
;

SLOW_RPT_UDLR	=	30
FAST_RPT_UDLR	=	10