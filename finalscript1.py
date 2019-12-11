import os
import RPi.GPIO as GPIO
import time
import serial
import wiringpi

#ser = serial.Serial(
#	port='/dev/serial0',#
#	baudrate = 9600,
#	parity=serial.PARITY_NONE,
#	stopbits=serial.STOPBITS_ONE,
#	bytesize=serial.EIGHTBITS,
#	timeout=1
#)#

wiringpi.wiringPiSetup()
serial = wiringpi.serialOpen('/dev/ttyAMA0',9600)
wiringpi.serialPuts(serial,0x02)
GPIO.setup(6, GPIO.OUT)
try:
	while True:
		GPIO.setmode(GPIO.BCM)
		GPIO.setup(13, GPIO.OUT)
		GPIO.setup(19, GPIO.OUT)
		GPIO.setup(26, GPIO.OUT)

		GPIO.setmode(GPIO.BCM)
		print(" ")
		print("1. Show Camera")
		print("2. Fire Turret")
		print("3. Rotate Left")
		print("4. Rotate Right")
		user_input = input("Enter a command:")
		if(user_input == "1"):
			os.system("raspivid -o - -t 1000000 -fps 12 -b 100000 -w 300 -h 300 |cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8021}' :demux=h264")
			time.sleep(10)
			#sys.exit()
		elif(user_input == "2"):
			print("Firing")
			GPIO.output(13,GPIO.HIGH)
			time.sleep(1)
			GPIO.output(13,GPIO.LOW)
			GPIO.cleanup()
#			ser.write(0)
			wiringpi.serialPuts(serial,'0x00')
		elif(user_input == "3"):
			print("Rotating Left")
			GPIO.output(19,GPIO.HIGH)
			time.sleep(1)
			GPIO.output(19,GPIO.LOW)
			GPIO.cleanup()
#			ser.write(0x03)
		elif(user_input == "4"):
			print("Rotating Right")
			GPIO.output(26,GPIO.HIGH)
			time.sleep(1)
			GPIO.output(26,GPIO.LOW)
			GPIO.cleanup()
#			ser.write(0x04)
		else:
			print("Error")
		print(" ")
except KeyboardInterrupt:	
	exit()



