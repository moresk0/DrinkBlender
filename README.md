# DrinkBlender

Drink Blender is my Bachelor thesis project. It is based on Arduino Nano and Android.

Control and communication between user and device was designed to be performed via Bluetooth and a special Android application created in the **MIT App Inventor**. The user selects a drink within the mobile application, and data containing type and quantity of drink is sent to the Arduino via Bluetooth
communication. Arduino executes the "recipe" and mixes the drink in the given quantities and ratios.

(All photos are in Croatian)

For liquid transfer 4 peristaltic pumps are used. As pumps need much more current than Arduino can provide, custom driver circuit is made.
![ControlC](https://raw.githubusercontent.com/moresk0/DrinkBlender/main/Photos/PWControl.png)

Circuitry is powered via wall adapter rated with 22 V @ 0,82 A. That voltage is dropped down across circuits. One is dropped to 12 V for powering pumps, and other to 5 V for powering Arduino.
![Power](https://raw.githubusercontent.com/moresk0/DrinkBlender/main/Photos/VoltageRegulation.png)

Complete schematic is shown below
![Schematic](https://raw.githubusercontent.com/moresk0/DrinkBlender/main/Photos/Schematic.png)

![Assembly](https://raw.githubusercontent.com/moresk0/DrinkBlender/main/Photos/Assembly.png)

#### Arduino Code FlowChart logic
![FlowChart](https://raw.githubusercontent.com/moresk0/DrinkBlender/main/Photos/FlowChart.png)
