## Pressure Sensor

## What does it do?
In a liquid chromatography system, the column represents a key component. Resins with specific properties tend to be expensive, and can be easily damaged by incorrect handling. 

The rate at which samples or liquids can be pushed through the column is not constant, and can be drastically reduced if a column is overloaded. If the flow rate is not reduced at this point, the column or other components can be damaged by the increasing back pressure.

## How is it implemented?
We decided to use the  MS5803-14BA sensor, as it can communicate by I2C and is protected against the liquid phase by a gel membrane cover. 

We bought a non-integrated sensor to minimize cost, but would advise against this course of action, as the SMD packaging format makes soldering difficult. We succeeded in soldering the sensor to an SMT breakoutPCB for SOIC-8 from Adafruit (Product nr. 1212), using a hot plate and a heat gun. For easier use, we would recommend the SparkFun Pressure Sensor Breakout Board MS5803-14BA (SEN-12909).

The sensor is integrated into the chromatography setup using a 3D printed part with a suitable cut-out for the sensor. The sensor and board were constrained by a clamp, and any gaps closed by silicone sealant. 

