# TTK4155 Embedded and Industrial Computer Systems Design
**Lab group**: 2\
**Group members:**
- Einar Augestad
- Edvard Dingsør
- Matheus Ullmann

## Node 1
### How to run tests
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