## Summary
Column chromatography is a routine technique for the separation of components from complex mixtures. In biochemistry and molecular biology, the most common application is the purification of recombinantly expressed proteins from in vitro or in vivo expression systems. These separations are currently performed by gravity flow, centrifugation or by using automated systems that use pumps to force solutions over a column. 
Gravity flow and centrifugation techniques, while simple and relatively inexpensive, are labour intensive and time consuming. Conversely, commercial systems allow a high degree of automation, limiting hands-on time, and allow more complex elution protocols, such as elution using salt or pH gradients. However, these systems require specialised, expensive equipment. The project proposed here aims to produce a lower cost, modular, open-source alternative to these commercial systems for performing simple, routine purifications.

## Proposal
Systems to be used and problems to be addressed
Research into protein structure and function relies heavily on the availability of highly pure proteins. These are routinely produced in recombinant expression systems and purified using a variety of chromatography methods, such as affinity purification, ion exchange or size exclusion chromatography.
 
There are a variety of methods available for applying samples and solutions to chromatography columns, each with their advantages and disadvantages. Gravity flow and spin column purification, while having the advantage of being relatively low-cost, are labour intensive, requiring manual sample and buffer addition and are also limited in terms of complexity of the elution conditions. Gravity flow and spin column purification are therefore mainly used for the purification of limited amounts of protein using various types of affinity purification. For large-scale purifications or purification requiring more complex elution conditions, such as salt or pH gradients, automated FPLC (fast protein liquid chromatography) systems are commonly used. These systems use pumps to force solutions over chromatography columns at pressure and thereby reduce run time. They also allow the use of preprogrammed complex purifications to take place without manual intervention. Their major drawback is their cost, with entry level models starting at roughly 5,000 GBP.
 
## Design goals and implementation 
We aim to create a system that sits in between the manual purification methods and the fully automated, specialised systems. The goals and proposed design steps for this system are as follows:

1)	Automate a gravity flow procedure with automated sample loading, washing and elution using a single elution buffer (step elution) for affinity purification. 
Initially, we will use three syringe pumps for wash solution, elution solution and sample solution respectively. These will be placed on a 3D-printed rack, to allow them to be moved with respect to the column. For the syringe-based pump system, we will adapt an existing open-source design, described here: (http://www.appropedia.org/Open-source_syringe_pump). We aim to allow for programming of the device via a simple UI on an embedded screen.

2)	Automated fraction collection
The fraction collector will take the form of a movable tube rack. The design will be based on a linear slider mechanism that will be either 3D printed or based on aluminium extrusions.

3)	Conversion of the gravity flow system to a pressurised closed system
We intend to use standard connectors (e.g. Luer) and silicone or Tygon tubing to connect the syringe pumps to the chromatography column. We will attempt to 3D print suitable connectors to maximize the usage of non-commercial parts. 

4)	On-line detection of protein using a custom-built detection module, display of results on fraction rack
To allow monitoring of the purification, we will add a detection module to the column outlet. This detection module will monitor protein concentration by absorbance at 280 nm in solution using a suitable UV LED and photodetector. Absorbance over time will be displayed on the embedded screen as well as on embedded LEDs in the fraction rack to indicate directly which tubes contain eluted protein.

More ambitious, additional goals, depending on success of initial designs would include:

1) Addition of valves/mixing chambers to the pump system to allow buffer mixing for running gradient elutions

2) Design of additional detection modules (e.g. DNA, absorbance at 260 nm) or fluorescence detection, which can be added to, or replace the 280 nm detector.

3) Addition of multiple pump and detector modules to allow parallel purification of multiple samples.

## Proposed outcomes and benefits
The major outcome of this project would be a low-cost, automated protein purification system, which could be used for simple, routine purifications in settings where gravity flow is too time consuming, but the cost of a commercial system is prohibitive. The open-source design will allow users to easily replace damaged parts as well as add to and customise the modules. Given the expected cost, we also see a use as a teaching tool for schools, undergraduate labs and biomaker spaces, to teach more advanced protein purification techniques and introduce students to the concept of automation in protein purification. The final 3D models, parts list, circuit diagrams, code and guides on replicating the project will be made available on Github for easy replication of the project.

