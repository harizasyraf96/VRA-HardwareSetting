Set WshShell = CreateObject("WScript.Shell")
WshShell.Run chr(34) & "C:\ESP32\start_receiver.bat" & chr(34), 0
Set WshShell = Nothing