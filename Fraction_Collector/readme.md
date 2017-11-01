 ## Fraction collector/SCARA Arm
 
 ## What does it do?
 A range of methods in a molecular biology laboratory is based splitting a sample in several sub-samples, commonly referred to as fractions. Depending on the method, fractions sometimes represent a spatial separation (e.g. when transferring liquids of ‘slices’ of a centrifugation tube into separate fractions), or temporal (e.g. when fraction are created from liquids running over a chromatography column). The common factor is that a certain amount of liquid is dispensed into a reservoir, then another one, then another one, and so on. Each of these fractions can then be treated independently  - to measure concentrations of compounds, enzymatic activities or just check what they smell like.
 
A fraction collector is fundamentally a piece of equipment that can hold a number of containers, and moves them about when required. 
 
 ## How is it implemented?
 
 Basic requirements:
-	use a flexible robotic arm with a small foot print when not in use
-	exchangeable tool head to accommodate a range of reservoirs

    o	SBS 96-well plate (shallow/deep)
    
    o	Eppendorf tubes
    
    o	Other reservoirs
    
-	Use common hardware from the 3D printing community
-	Focus on 3D printed parts to minimize commercial parts
-	Make something more exciting than a standard 2-axis plotter

We decided to use a 2D SCARA design (Selective Compliance Articulated Robot Arm) that was designed from scratch with an eye on minimizing exotic hardware. It is powered by NEMA 17 servos and Arduino hardware, using driver boards from the RepRap 3D printing community and cheap GT2 belts with 3D printed belt clamps. In order to save cost, we avoided optical or other encoders, and solely rely on an initial calibration step using microswitches to determine the absolute position of the arm. 

A carrier plate at the ‘wrist’ of the arm can hold a number of reservoirs, with the primary design focusing on standardized 96 well plates in an SBS format.  Other holders can be easily designed and programmed for.

The open source design should allow easy integration and adoption of this fraction collector for purposes other than chromatography
 
 ## Description of files
 Will be added when finished.
 
 ## Future work/Known bugs
 Future work:
 - add support for Eppendorff tubes and other reservoirs, other than the 96-well plate added so far
 
 Known bugs:
 - fraction collection is not accurate due to the code not taking into account the angle of the arm when aiming for the wells. Work is in progress on fixing the code
 
