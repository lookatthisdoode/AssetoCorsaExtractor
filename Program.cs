using AssettoCorsaSharedMemory;
using Newtonsoft.Json;
using System;
using System.IO.Ports;

namespace AssetoCorsaExtractor
{
    class Program
    {
        static SerialPort serialPort;
        static AssettoCorsa ac;

        static void Main(string[] args)
        {
            try
            {
                serialPort = new SerialPort("COM5", 9600); // Adjust COM port and baud rate as needed

                // Enable communication back
                serialPort.RtsEnable = true;

                // Subscribe to the DataReceived event to handle incoming messages
                serialPort.DataReceived += SerialPort_DataReceived;
                serialPort.Open();

                AssettoCorsa ac = new AssettoCorsa();
                ac.PhysicsUpdated += ac_PhysicsInfoUpdated; // Add event listener for StaticInfo
                ac.StaticInfoInterval = 5000; // Get StaticInfo updates every 5 second
                ac.StaticInfoUpdated += ac_StaticInfoUpdated;
                ac.Start(); // Connect to shared memory and start interval timers 

                Console.WriteLine("Assetto Corsa running. Press any key to exit.");
                Console.WriteLine("Hold Gear Up for 1 sec to reset RPMs");

                // Waiting for any key to close the app
                // I guess this is what keeps it running
                Console.ReadKey();
            }
            catch (Exception ex)
            {
                Console.WriteLine("An error occurred: " + ex.Message);
            }
            finally
            {
                serialPort?.Close();
                ac?.Stop();
            }
        }

        static void ac_PhysicsInfoUpdated(object sender, PhysicsEventArgs e)
        {
            // Create data message object
            var dataMessage = new { method = "data", currentRpm = e.Physics.Rpms };

            // Serialize data message to JSON
            string dataMessageJson = JsonConvert.SerializeObject(dataMessage);

            // Send data message over serial port
            SendDataOverSerial(dataMessageJson);
        }

        static void ac_StaticInfoUpdated(object sender, StaticInfoEventArgs e)
        {
            // Create setup message object
            var setupMessage = new { method = "setup", maxRpm = e.StaticInfo.MaxRpm };

            // Serialize setup message to JSON
            string setupMessageJson = JsonConvert.SerializeObject(setupMessage);

            //Console.WriteLine(setupMessageJson);

            // Send setup message over serial port
            SendDataOverSerial(setupMessageJson);
        }

        static void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                // Read the incoming message from the serial port
                string message = serialPort.ReadLine();

                // Display the received message in the console
                Console.WriteLine("ARDUINO: " + message);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error receiving data from serial port: " + ex.Message);
            }
        }

        static void SendDataOverSerial(string data)
        {
            try
            {
                // Send data message over serial port
                serialPort.WriteLine(data);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error sending data over serial port: " + ex.Message);
            }
        }
    }
}
