import asyncio
import serial_asyncio

class SerialReader(asyncio.Protocol):
    def connection_made(self, transport):
        self.transport = transport
        print("Serial port opened:", transport)

    def data_received(self, data):
        # Process incoming data immediately
        print("Received:", data.decode('utf-8').strip())

    def connection_lost(self, exc):
        print("Serial port closed")
        asyncio.get_event_loop().stop()

async def main():
    loop = asyncio.get_running_loop()
    # Replace '/dev/ttyACM0' with your actual serial port.
    await serial_asyncio.create_serial_connection(loop, SerialReader, '/dev/ttyACM0', baudrate=115200)

asyncio.run(main())
