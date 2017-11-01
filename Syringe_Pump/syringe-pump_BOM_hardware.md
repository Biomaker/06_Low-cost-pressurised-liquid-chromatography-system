Syringe Pump

Bill of materials 

-	Core 3D printed parts
-	1 x Stepper Motor NEMA-17 Size - 200 Steps/Rev, 12V 350mA
-	ProtoShield Protoyping Board for Arduino
-	1 x StepStick 4-layer DRV8825 Stepper Motor Driver boards
-	12V DC Connector 2.1x5.5mm DC Power Cable Jack Adapter
-	
-	3 x button microswitch
-	1 x 5 mm LED
-	2 x 30 cm 6mm steel rod 30 cm (depending on syringe)
-	1 x 30 cm M5 threaded rod 30 cm
-	1 x shaft coupler M5 
-	2 x LM6UU linear bearing 6 mm
-	1 x M5 knurled thumb nut, thin type (or other flat, wide M5 nut)
-	Various M3 screws for assembly 

Notes:

Top plate: print with 10-20% infill to create a sturdy structure. The linear bearings should fit snugly into the slots, 
same with the thumb screw (perhaps fix all with a dab of glue once the threaded rod holds it in place). 
The current version is designed for most 50/60ml syringes, but we have encountered syringes with unusual 
thickness/shape at the top of the plunger that wouldn’t fit – carving for a few minutes with a Stanley 
knive (on the syringe, not the 3D printed part) usually solved any problems.

Syringe adapter: The luer loc doesn’t fit perfectly yet, and will probably leak. 
Install a brass nut insert at the bottom (use a mixture of heat (soldering iron) and epoxy to fuse it tightly into the PLA), 
and then install the tubing with the printed connector screw, a rubber o-ring and any other sealing helpers (silicone, Teflon tape).  
Verify water tight fit, then install in the base unit. 


Base assembly: This is somewhat tight. A standard Arduino Uno just about fits into the base, and will support some of the pressure from the top. 
The vertical rods are supposed to be connected to two wires to carry power to the emergency off (maximum) button switch 
sitting on the holder. Conductive ink helps connector the wires in the 6mm rod slots to the rods themselves, otherwise soldering is an option. 
Similarly, the horizontal clamp has a slot for a short wire & a button switch to allow the pump to sense when it shouldn’t move any lower.
Install the two button switches & the LED at the front of the base.
Mount the Arduino against the vertical wall with two screws.
Fix the 12V connector in place with the clamp (and/or some glue). 
Place the finished prototyping board on top, then carefully close up the base. 

If it doesn't click into place, check for wires that got in the way. 
