import serial
import numpy as np

with serial.Serial('COM4', 115200) as ser:
    while True:
        arr = np.array([], dtype=np.uint8)
        income = ser.read()
        while income != b'\n':
            arr = np.append(arr, np.frombuffer(income, dtype=np.uint8))
            income = ser.read()

        else:
            print(arr)
            print(arr.dtype)
            print(len(arr))
            with open("data.raw", "ab") as file:
                file.write(arr)

