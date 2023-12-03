import serial
serialport = serial.Serial("/dev/ttyUSB0", 9600, timeout=0.5)

while(True):
	print(serialport.readlines(None))
