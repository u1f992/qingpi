# NeoControllerMod

NeoControllerMod is a project for externally controlling the Nintendo 3DS series via serial communication.

- PCB, FPC, firmware and sample applications included.
  - Small enough to fit in a original housing.
  - Able to install with [LL-SPA3](https://optimize.ath.cx/LL-SPA3/index.htm) by Optimize.
  - Using common components like RP2040 and easy to manufacture.
- Buttons, slide pad, and touch screen supported, not impair the original function.
  - IR not yet supported, but connected (contributions welcome).
- Aim to be fully documented.

Currently provided only for 3DS XL, but the basic idea can be applied to Old 3DS/New 3DS/2DS series.

## Prehistory

Automatic control of the 3DS by a microcontroller has already been realized with [3DS controller mod](https://3dscapture.com/controller/) by loopy or [3xtDS](https://github.com/dekuNukem/3xtDS) by dekuNukem, both of which are currently difficult to obtain or produce. Heard that outside of Japan it is common to use CFW for external control (and video), and hardware modifications may be seen as no longer necessary.

For some reason, however, in Japan software mod is strongly condemned, but hardware mod tends to be tolerated (as in the case of the SPA3, called "偽トロ" and tolerated among fans). That is why this project was born.

## Disclaimer

This is a fan project and the author assumes no responsibility for any 3DS hardware modifications. Use at your own risk and discretion.
