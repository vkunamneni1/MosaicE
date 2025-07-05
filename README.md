# MosaicE
MosaicE is a PCB controller board that's powered by a XIAO RP2040, three buttons, a rotary encoder, and four LEDs. There are 3 modes: clock, binary counter, and gradient light mode. All modes are controlled by user input through the buttons and knob, and are all programmed in C++ (Arduino code).

## 3D Model
![Screenshot 2025-07-04 at 10 31 22 PM](https://github.com/user-attachments/assets/0d696eda-a47b-446c-a703-f3b2c8356bf3)
![Screenshot 2025-07-04 at 10 33 12 PM](https://github.com/user-attachments/assets/08fead19-47de-468e-8f24-1303049d8a52)
(models found on the GrabCAD Library and combined in KiCad)

## PCB Design
![Screenshot 2025-07-04 at 10 32 26 PM](https://github.com/user-attachments/assets/0fbf03c0-b2ee-4cfd-99e9-03d7f768ba20)
![Screenshot 2025-07-04 at 10 32 43 PM](https://github.com/user-attachments/assets/71d4ea25-4ee7-4cc8-b3e6-84e4965f6866)
(made in KiCad)

## Schematic
![Screenshot 2025-07-04 at 10 33 33 PM](https://github.com/user-attachments/assets/51fe94ca-7d3f-4056-96ef-1126728b5e31)
(made in KiCad)

## Firmware
Source files can be found in the firmware directory. It was made in the Arduino IDE, using third-party libraries such as RotaryEncoder by Matthias Hertel.

##### made for Pathfinder
