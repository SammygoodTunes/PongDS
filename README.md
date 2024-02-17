# PongDS

## Explanation:

This is a port of the Python pong game I made a while back, made specifically to run on the Nintendo DS.

The top screen shows the game, the bottom screen displays the score.

### Future updates:

- Use libnds' built-in sinLerp() and cosLerp() (fixed point)

- Multiplayer mode (Wireless/LAN)

- Better music

- Power-ups / Obstacles (to make the game interesting)

## Controls:

- [**START**] -> Start game


- [**DPad Up/Down**] -> Move your paddle

## Compilation:

In order to compile the source, you will need the devkitPro.


Then, in the root folder, run the Makefile with: `make`

Running the game requires either an emulator (No$gba, DeSmuME, melonDS...), either an R4 card or either a homebrewed DS.

> Though some emulators may work with this ROM, others (very likely) will not support homebrew games as it is very difficult to emulate a ROM built using libnds.

## Development information:

Developed by: SammygoodTunes


Library(ies) used: libnds, maxmod9
