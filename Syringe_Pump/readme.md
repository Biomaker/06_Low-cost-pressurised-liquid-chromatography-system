## Syringe pumps

## What does it do?
A liquid chromatography setup relies on the controlled flow of liquid over a chromatography column. To provide this flow, we implement two syringe pumps, which can be programmed to flow at a desired flow rate. The system has two pumps, connected via a mixing chamber to potentially allow the use of a mixture of two buffers at ratios defined by the respective flow rates.

## How is it implemented?

## Description of files
Syringe_Pump_3_0_pump1 and Syringe_Pump_3_0_pump2 -> Arduino sketches, each controlling one pump. The only difference between the two sketches is the I2C address, which allows the pumps to be controlled from the control unit.
3D printer files -> contains CAD and 3D printer files for the syringe pump hardware

## Future work/Known bugs

