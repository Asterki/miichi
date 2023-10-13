# Mi-ichi
Mi-ichi is a robot, made in C++ and the WEMOS LOLIN32 Lite board

## Features
- Forwards and backwards walking
- 5 Different positions
- Connection through Bluetooth 

## Planned Features
- Integrated speaker
- AI controlled robot
- Stabilization using gyroscope
- More positions and movements

## Requirements 
The hardware requirements can all be satisfied by getting [the robot](https://en.aliexpress.com/item/1005002261067786.html), also a USB Micro b and 3.7v batteries.

Software requirements:
- A device capable of Bluetooth v4.4
- Arduino IDE

## Running the code
To do this, we can start by clonning into this repository
```bash
$ git clone https://github.com/Asterki/miichi.git
```
Enter to the newly created folder
```bash
$ cd miichi
```
Edit the code using Arduino IDE
```bash
$ arduino-ide miichi.ino
```
Then just upload the code, turn on the device, and you're ready to pair and control

## Positions
- 1: Stand up
- 2: Sit down
- 3: Lay down
- 4: Stretch
- 5: Walk
- 6: Walk backwards
- 7: Stop walking
Send the corresponding number through a paired and connected bluetooth serial, and the robot will move to that position/do that movement.
