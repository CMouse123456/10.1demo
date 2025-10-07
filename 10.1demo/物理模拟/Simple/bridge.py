import serial
import time
import mmap
import struct
import os

ser = serial.Serial(
    port = '/dev/ttyUSB0',
    baudrate=9600,
    timeout=1
)

x = 0
y = 0
button = 0

# 使用文件映射
shm_name = "/dev/shm/joy_stick_data"
shm_size = 12 # int * 3

if not os.path.exists(shm_name):
    with open(shm_name,"w+b") as f:
        f.write(b"\x00" * 12)

try:
    with open(shm_name, 'r+b') as f:
        shm = mmap.mmap(f.fileno(), shm_size, access=mmap.ACCESS_WRITE)
        print("reading...")
        while True:
            if ser.in_waiting:
                data = ser.readline().decode("utf-8", errors='ignore')
                if data.startswith("J"):
                    data_list = data.split(",")
                    x = int(data_list[1])
                    y = int(data_list[2])
                    button = int(data_list[3][0])
                    
                    # write in memory
                    shm.seek(0)
                    shm.write(struct.pack('<iii', x, y, button))
                    shm.flush()

                    print("写入:  ", x, y, button)

                    time.sleep(0.001)

            else:
                time.sleep(0.01)
        
except KeyboardInterrupt:
    print("user out")

finally:
    ser.close()
    print("close")
        