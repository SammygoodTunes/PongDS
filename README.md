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

Though the ROM is already provided in the repository, some may want to compile from source.

In order to do so, you will need [devkitPro](https://github.com/devkitPro/installer/releases).

Then, in the root folder, run the Makefile with: `make`

Running the game requires either an emulator (No$gba, DeSmuME, melonDS...), either an R4 card or either a homebrewed DS.

> Though some emulators may work with this ROM, others (very likely) will not support it as homebrew games are difficult to emulate.

## Development information:

Developed by: SammygoodTunes

Libraries used: libnds, maxmod9
