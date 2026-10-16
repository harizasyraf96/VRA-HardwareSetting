# VRA-HardwareSetting
ESP32 transmitter (remote) and ESP32 receiver (PC interface) for dual‑monitor Visual Reinforcement Audiometry.

## Files Overview

- **receiver.py**  
  Listens to commands from the ESP32 receiver via serial port and launches VLC to play reinforcement videos.

- **monitors.py**  
  Handles dual-monitor setup, ensuring videos open on the correct screen (left/right).

- **start_receiver.bat**  
  Batch script to quickly start the Python receiver without typing commands manually.

- **start_receiver.vbs**  
  VBScript wrapper to run the batch file silently (no console window).
