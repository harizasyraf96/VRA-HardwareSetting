import os

import serial

import subprocess
import time
import win32gui
import win32con

#create once at startup

VLC = r"C:\Program Files\VideoLAN\VLC\vlc.exe"
VIDEO = r"C:\Video\video__01.mp4"

print("VLC Exists:", os.path.exists(VLC))
print("Video Exists:",os.path.exists(VIDEO))

ser = serial.Serial('COM4', 115200)

print("Listening...")

def stop_vlc():
	subprocess.run(
		["taskkill", "/F", "/IM", "vlc.exe"],
		capture_output=True
	)

def play_left():
	stop_vlc()
	subprocess.Popen([VLC, VIDEO, "--qt-start-minimized"])
	time.sleep(0.3)

	def callback(hwnd, windows):
		title = win32gui.GetWindowText(hwnd)

		if "VLC media player" in title:
			windows.append(hwnd)

	windows = []
	win32gui.EnumWindows(callback, windows)

	if windows:
		hwnd = windows[0]

		win32gui.SetWindowPos(
			hwnd,
			None,
			0,	# Monitor 2
			0,
			1280,
			800,
			win32con.SWP_SHOWWINDOW
		)
		win32gui.ShowWindow(hwnd, win32con.SW_MAXIMIZE)
	print("LEFT SCREEN")

def play_right():
	stop_vlc()
	subprocess.Popen([VLC, VIDEO, "--qt-start-minimized"])
	time.sleep(0.3)
	def callback(hwnd, windows):
		title = win32gui.GetWindowText(hwnd)
		if "VLC media player" in title:
			windows.append(hwnd)
	windows = []
	win32gui.EnumWindows(callback, windows)
	if windows:
		hwnd = windows[0]
		win32gui.SetWindowPos(
			hwnd,
			None,
			1280,	# Monitor 1
			0,
			1280,
			800,
			win32con.SWP_SHOWWINDOW
		)
		win32gui.ShowWindow(hwnd, win32con.SW_MAXIMIZE)
	print("RIGHT SCREEN")

while True:

    line = ser.readline().decode(errors="ignore").strip()

    if not line.startswith("CMD:"):
        continue

    print("Received:", line)

    if line == "CMD:1,1":
        print("LEFT SCREEN")
        play_left()

    elif line == "CMD:2,1":
        print("LEFT TOY")

    elif line == "CMD:3,1":
        print("LEFT BOTH")

    elif line == "CMD:1,2":
        print("RIGHT SCREEN")
        play_right()

    elif line == "CMD:2,2":
        print("RIGHT TOY")

    elif line == "CMD:3,2":
        print("RIGHT BOTH")

    elif line == "CMD:4,0":
        print("STOP")
        stop_vlc()
