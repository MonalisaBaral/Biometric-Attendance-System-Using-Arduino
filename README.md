# Biometric Attendance System Using Arduino

## Project Overview
The **Biometric Attendance System** is designed to automate attendance tracking by using fingerprint recognition. This system is based on an Arduino Uno microcontroller paired with an R307 fingerprint sensor. It registers attendance by verifying fingerprints and logs the data with timestamps. The project is aimed at schools, colleges, offices, and other institutions where reliable and accurate attendance tracking is essential.

### Key Features
- **Fingerprint registration and matching**: Allows up to 256 fingerprint records.
- **LCD display**: Shows attendance status and prompts for user actions.
- **Real-Time Clock (RTC)**: Logs the exact time and date of attendance.
- **Cloud integration**: Stores attendance records for further processing or retrieval.
- **Push-button interface**: For enrolling or deleting fingerprints.
- **No proxy attendance**: Secure biometric identification.

## Components Used
- **Arduino Uno**
- **R307 Fingerprint Module**
- **16x2 LCD Display**
- **DS3231 RTC Module**
- **Pushbuttons (4)**
- **LEDs (2)**
- **Jumper wires and breadboard**
  
### Circuit Diagram
![Circuit Diagram](https://github.com/MonalisaBaral/Biometric-Attendance-System-Using-Arduino/blob/main/circuit_diagram.jpg)

## Software Requirements
- **Arduino IDE**: [Download here](https://www.arduino.cc/en/software)
- **Adafruit Fingerprint Sensor Library**: For interfacing with the R307 fingerprint sensor.
- **RTClib**: For working with the DS3231 Real-Time Clock module.
- **EEPROM**: To store fingerprint and attendance data.

### How to Install Libraries
1. Open Arduino IDE.
2. Go to `Sketch` > `Include Library` > `Manage Libraries`.
3. Search for "Adafruit Fingerprint Sensor Library" and "RTClib" and install them.

## Setup Instructions
1. **Clone the repository**:
   ```bash
   git clone https://github.com/MonalisaBaral/Biometric-Attendance-System.git
   ```
2. Open the `.ino` file located in the `src/` folder using the Arduino IDE.
3. **Hardware Setup**:
   - Connect the Arduino with the fingerprint module, RTC, LCD, and pushbuttons as per the circuit diagram.
4. **Upload Code**:
   - Select the correct COM port for your Arduino.
   - Click the upload button to load the code onto your Arduino board.
5. **Library Installation**:
   - Ensure the necessary libraries are installed as mentioned above.
6. Once the code is uploaded and hardware is set up, follow the on-screen instructions displayed on the LCD for registering or verifying fingerprints.

## How It Works
1. **Registration**: Use the pushbuttons to register a fingerprint. The system will store the fingerprint ID and associate it with the user.
2. **Attendance Logging**: When a registered user places their finger on the sensor, the system checks the fingerprint and logs their attendance along with the time and date.
3. **Display**: The 16x2 LCD will display the user's ID and attendance status (e.g., "Present", "Not Found").
4. **Delete Fingerprints**: Users can delete stored fingerprints by following the on-screen instructions.

### Pushbutton Functions:
- **Register Button**: Enroll new fingerprints.
- **Delete Button**: Remove stored fingerprints.
- **Forward/Reverse Buttons**: Navigate through stored fingerprints.

## Code Explanation
The project uses the `Adafruit Fingerprint Sensor Library` for reading and matching fingerprints. The `RTClib` is used to fetch the real-time clock data for timestamping the attendance logs. All actions and status messages are shown on the LCD. 

- **Fingerprint Enrollment**: New fingerprints are enrolled by pressing the register button and following the prompts.
- **Fingerprint Matching**: Attendance is logged when a matching fingerprint is found.
- **Data Storage**: Fingerprints and logs are stored in the Arduino’s EEPROM and can be uploaded to the cloud for long-term storage.

## Future Enhancements
- **Add Email/SMS Notifications**: Notify the user when attendance is logged.
- **Cloud Integration**: Extend the project to sync data with cloud services like Google Sheets or a custom database.
- **Mobile App**: Develop a mobile application to track attendance records.

## Troubleshooting
- **Fingerprint not recognized**: Ensure the finger is placed correctly on the sensor and that it is registered.
- **LCD not displaying correctly**: Check wiring connections between the Arduino and LCD.
- **RTC not updating**: Verify the battery in the DS3231 module and ensure the library is installed correctly.

## References
- [How to make a Fingerprint-Based Attendance System](https://maker.pro/arduino/projects/how-to-make-a-fingerprint-based-attendance-system-with-arduino-and-r305)
- [Adafruit Fingerprint Sensor Library](https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library)
  
## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
```
