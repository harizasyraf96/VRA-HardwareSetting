# VRA-HardwareSetting
ESP32 transmitter (remote) and ESP32 receiver (PC interface) for dual‑monitor Visual Reinforcement Audiometry.

## Files Overview

DOWNLOAD file named ESP32

- **receiver.py**  
  Listens to commands from the ESP32 receiver via serial port and launches VLC to play reinforcement videos.

- **monitors.py**  
  Handles dual-monitor setup, ensuring videos open on the correct screen (left/right).

- **start_receiver.bat**  
  Batch script to quickly start the Python receiver without typing commands manually.

- **start_receiver.vbs**  
  VBScript wrapper to run the batch file silently (no console window).

DONWLOAD IDE file named ESP_NOW_RECEIVER, upload into ESP32 which will acts as receiver on local (connected to COMM port)
