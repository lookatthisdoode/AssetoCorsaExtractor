# AssetoCorsaExtractor

## Overview
AssetoCorsaExtractor is a C# console application that extracts telemetry data from Assetto Corsa via shared memory and sends it to an Arduino microcontroller over a serial connection. The Arduino then controls peripherals like LEDs and buttons based on the received telemetry data.

## Features
- Extracts telemetry data such as RPMs and car setup information from Assetto Corsa.
- Sends telemetry data to an Arduino microcontroller over a serial connection.
- Controls LEDs and buttons on the Arduino based on the received telemetry data.
- Supports setup and data messages sent between the console application and the Arduino in JSON format.

## Usage
1. Install the necessary dependencies:
   - Visual Studio (or another IDE that supports C#).
   - Arduino IDE (if modifying the Arduino code).
   - Assetto Corsa game installed on your PC.

2. Set up the hardware:
   - I use Arduino gear shifter, `Asseto_Corsa_Extractor.ino` file in `arduino` folder should explain what im doing.
   - Adjust Arduino firmware for you own case.
   - Connect your Arduino microcontroller to your PC via USB.
   - Connect any peripherals (LEDs, buttons, etc.) to the Arduino as needed.

3. Build and run the AssetoCorsaExtractor application:
   - Open the solution in Visual Studio.
   - Set up the COM port and baud rate for the serial connection in the `Program.cs` file.
   - Build and run the application.

4. Launch Assetto Corsa:
   - Start the Assetto Corsa game on your PC.

5. Enjoy:
   - The console application will extract telemetry data from Assetto Corsa and send it to the Arduino.
   - In my case arduino will act as a controller, emulating 2 buttons(gear up/down) and LED would light up when revvs too high.

## Contributing
Contributions are welcome! If you have any ideas for improvements or new features, feel free to submit a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
