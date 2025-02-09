Description
This Arduino-based door locking system enhances security by using a keypad for authentication. It allows users to unlock the door by entering a correct PIN and can be monitored remotely using IoT integration.

Features
Keypad-based authentication
Remote monitoring (with IoT)
Arduino-powered
Buzzer and LED alerts for success/failure
Customizable PIN
Components Used
Arduino Uno (or ESP8266/ESP32)
4x4 Keypad
Solenoid Lock
Relay Module
Buzzer & LED
Setup
Clone the repository:
sh
Copy
Edit
git clone https://github.com/yourusername/IoT-Keypad-Door-Lock.git
cd IoT-Keypad-Door-Lock
Open the door_lock.ino file in Arduino IDE and upload it to your board.
Follow the circuit connections as shown in the diagram.
Usage
Enter the PIN on the keypad to unlock the door.
Buzzer will sound for incorrect attempts.
