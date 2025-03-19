import serial

# Open the serial port that the Arduino is connected to.
# Check the port name; it might be '/dev/ttyACM0' or '/dev/ttyUSB0'
ser = serial.Serial('/dev/ttyACM0', 9600)

while True:
    try:
        line = ser.readline().decode('utf-8').strip()  # Read and decode the line
        if line:
            print("Received:", line)
    except Exception as e:
        print("Error:", e)
