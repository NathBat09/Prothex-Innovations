import serial
import threading

def read_serial(ser):
    while True:
        try:
            # Read all available data without blocking too long.
            if ser.in_waiting:
                line = ser.readline().decode('utf-8').strip()
                if line:
                    print("Received:", line)
        except Exception as e:
            print("Error:", e)

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=0.1)
thread = threading.Thread(target=read_serial, args=(ser,))
thread.daemon = True  # Allows the program to exit even if thread is running
thread.start()

# Main loop can perform other tasks without delay.
while True:
    # Replace this with other processing
    pass
