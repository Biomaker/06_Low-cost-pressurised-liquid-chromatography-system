## Control unit

## What does it do?
The control unit coordinates the function of the other components of the FPLC. It checks if all devices are connected and provides feedback. It then sends a signal to the fraction collector to start its calibration. Users can then choose from a number of options to run the FPLC system either manually, or make use of pre-programmed methods.

To save on Arduinos and reduce communication, the control unit also reads out the values from the pressure and UV sensors in our implementation. 

## How is it implemented?
The control unit communicates with the other devices over I2C. For display, it uses the Seeed kit Grove RGB LCD. To navigate the menus, it makes use of five buttons, connected via different resistors to one analog pin on the Arduino. Depending on the analog value received, the Arduino can determine which button has been pressed.

The menu layout is as follows:
1) Start/Welcome screen
2) Check I2C communication to all connected devices and display if devices are connected - allow user to repeat the test if desired
3) Start fraction collector calibration
4) Allow choice between Manual Run or Method Run:

5) Manual Run - Select either which pump to run at which speed -> this will trigger the selected pump and tell it to run at the speed specified via I2C, or select Display sensor values. Display sensor values will display current values from the pressure sensor (in bar) and the UV sensor

6) Method Run - Currently the unit has two pre-programmed methods- a pump wash method, which will first run pump 2 and then pump 1 to wash out residual buffer from a previous run and prefill the tubing with the solution from pump 1. The second method is a standard run method, which first runs pump 1, pauses to allow loading of the sample, runs pump 1 while collecting fractions and then runs pump 2 while collecting fractions. During the entire program, UV and pressure sensor readings are displayed. The current program assumes a 1 ml column.

For implementation of the UV sensor and pressure sensor, see the respective subfolders.

## Description of files
Control_unit_1_4                      -> latest Arduino sketch for running the control unit
MCU v3.step                           -> CAD file for control unit enclosure
MCU_base.stl and MCU_cover.stl        -> 3D printer files for enclosure

## Future work/Known bugs/issues

Future work:
- Bug fixing - see below
- Allow saving of the UV and pressure sensor information to an SD card
- Allow display of the UV and pressure sensor information as it relates to the fractions after the run
- Build a display and analysis program on a Raspberry Pi for display and processing of run data
- Allow user to add pre-programmed methods using the display and analysis program

Known bugs:
- UV sensor readings are not very sensitive and too low to be of use - could potentially be fixed by reducing the flow path distance and using a more powerful UV light source
