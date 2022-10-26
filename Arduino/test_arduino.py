from time import sleep
import os


try:
    from win32api import GetAsyncKeyState
    from win32con import VK_LBUTTON
    import serial
except:
    os.system('pip install pywin32')
    os.system('pip install pyserial')
    from win32api import GetAsyncKeyState
    from win32con import VK_LBUTTON
    import serial


if __name__ == '__main__':
    IS_CONNECTED = False
    port_num = 0  # Or use your own port number
    while True:
        try:
            arduino = serial.Serial('COM'+str(port_num), 115200, timeout=1)
            break
        except:
            port_num += 1

    arduino.write('arduino'.encode())
    sleep(0.001)
    if (reader:=arduino.read(arduino.inWaiting()).decode('utf-8')) == 'arduino':
        print('Connect success !!!')
        IS_CONNECTED = True
        sleep(0.2)
    else:
        print(reader)

    if IS_CONNECTED:
        arduino.write('m(-50,-50)'.encode())
        sleep(0.001)
        arduino.write('m(-50,-50)'.encode())
        sleep(0.001)
        arduino.write('m(50,50)'.encode())
        sleep(0.001)
        arduino.write('p(1)'.encode())
        sleep(0.001)
        print('Left button is pressed', GetAsyncKeyState(VK_LBUTTON)<0)  # check if left pressed
        sleep(1)
        arduino.write('m(50,50)'.encode())  # should move to starting point
        sleep(0.1)
        arduino.write('r(1)'.encode())
        sleep(0.1)
        arduino.write('d(a)'.encode())
        sleep(1)
        arduino.write('u(a)'.encode())
        sleep(0.1)

    arduino.close()
