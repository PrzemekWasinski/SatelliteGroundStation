# Weather Satellite Ground Station
This is my weather satellite ground station which is mounted on my garden shed, it automatically records radio signals from passing weather satellites and decodes them into satellite imagery. 

The C++ program is used to call the N2YO API to receive the time and angle of satellite passes. Each satellite pass is saved in `data/satellite_passes.csv`, while the program is running this file is read and if a satellite pass is about to start the script starts recording radio signals at 137.9MHz. After the satellite has passed the recording is then decoded using SatDump into weather satellite imagery.

<img width="450" height="600" alt="20261117_093750" src="https://github.com/user-attachments/assets/5fe7926b-30f3-4589-9e95-62b9e938afb6" />

<img width="450" height="600" alt="20261010_153111" src="https://github.com/user-attachments/assets/ec10f3ad-c0d5-49b6-a544-5a674940864f" />

## Hardware
The project runs on a Raspberry Pi 4 which sits in the lower outdoor enclosure, the enclosure is cooled with 2 mini fans both powered by the Pi 4's GPIO pins. For both fans the air travels through a downward tilted PVC angled corner pipe so rain water cannot enter the enclosure, each PVC pipe entrance and exit is lined with aluminium mesh to stop debris from entering. The pi 4's CPU is cooled with a mini CPU fan also powered by the GPIO pins.

For the SDR I went with the RTL-SDR Blog V4 which lives in the upper enclosure. At first the SDR was housed in the same enclosure as the Pi 4 (as you can see in image 2) but this set up had extremely bad radio reception, it took me days of troubleshooting to realise that the Pi 4 was emitting its own radio signals which drowned out the weaker satellite radio signals.

For the antenna I used a basic V-Dipole antenna I had laying around, the junction is housed in a small waterproof enclosure to prevent rust while each arm is inserted into a 60cm PVC pipe with plastic bottle caps blocking the pipe ends. Each antenna arm is extended to 53.4cm and the arms are spread to 120° and pointed north.

All of the hardware is connected in this configuration:

Raspberry Pi 4 -> 2m USB extension cable -> RTL-SDR Blog V4 -> V-Dipole Antenna

# Tech Stack
    Language: C++
    Host: Raspberry Pi 4 8gb with Raspberry Pi OS 64 bit
    Receiver: RTL-SDR Blog V4
    Antenna: V-Dipole tuned to 137.9 MHz
