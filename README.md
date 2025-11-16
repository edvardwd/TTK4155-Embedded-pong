https://github.com/edvardwd/TTK4155-Embedded-pong

# TTK4155 Embedded and Industrial Computer Systems Design
**Lab group**: 2\
**Group members:**
- Einar Augestad
- Edvard Wekre Dingsør
- Matheus Ullmann

## Project info
This repository contains code for the [TTK4155 Embedded and Industrial Computer Systems Design](https://www.ntnu.edu/studies/courses/TTK4155) course at NTNU, and answers the lab assignments for the fall semester of 2025. The project implements a simple pong game where two nodes communicate over CAN bus to play the game. `Node 1` handles user input through an IO-board while `Node 2` handles the motor, solenoid and servo control.



### How to run Node 1 tests
Open a new terminal and run:
```bash
picocom /dev/ttyS0 -b 9600
```
In another terminal:\
From the parent directory:

```bash
cd node1
./run_tests.sh
```
>NOTE: Might need to run `chmod +x run_tests.sh` first