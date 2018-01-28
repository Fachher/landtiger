import serial
port = serial.Serial("/dev/tty.wchusbserial1420", baudrate=9600, timeout=0.2)

while True:
	rcv = port.readline()
	if len(rcv) > 10:
		print "Tag detected: " + rcv
