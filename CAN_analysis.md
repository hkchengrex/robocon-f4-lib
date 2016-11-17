# CAN Analysis

Test Platform:
2 F4 board with 1 sender and 1 receiver

Method of testing:
The sender would generate continuous data stream with an increasing integer. That number is copied twice to a 8-btye CAN message packet. Those packets would be sent with a constant data speed.

The receiver would check whether the incoming data are in correct order (by checking the integer), and whether the two copies of the integer number is the same. The receiver would also display the data rate.

If the data are not in the correct order, there will be a **TIME ERROR**. If the two copies are not the same, there will be a **CONSISTENCY ERROR**.

## Inital version 
A modified version (to adapt F4) of Kenneth's version.

### Performance
Time error does not exist below 50 KB/s. When tuned >60 KB/s, time error happens ~10 times every second. THe sender board sometimes die.