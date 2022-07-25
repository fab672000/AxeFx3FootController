# AxeFx3FootController
Economic Arduino MIDI Controller for the Fractal Audio Axe-FX III.
More details on:
https://forum.fractalaudio.com/threads/diy-axe-fx-foot-controller-for-50-open-project.119882/

This Project was originally forked from the excellent Pinfloid's original forum work and branch: Axe3ContLCD11.01 (https://github.com/Pinfloid/Axe3ContLCD11.01) but turned rapidly into a fairly different, now 100% c++ project.

Initial project goals / ideas:
=============================

Allowing arduino capable builders to:

- Build a minimum, economical midi floorboard for guitarists using the FAS products, that can evolve rapidly (eventually in future for supporting more products)
- Support the hardware: more displays, more arduino compatible boards
-Reduce memory footprint to make it work with more Arduino compatible boards
- Introduce new ideas and architecture compared to Pinfloid original such as:
   - autorepeat and acceleration when pushing a button (e.g. program next/previous change )
   - re-purpose the existing buttons layout to support with only 2 rows of switches features such as tuner and loop mode.

   