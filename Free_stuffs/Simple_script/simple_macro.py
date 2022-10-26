from win32con import VK_LBUTTON, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_F1, VK_F2
from sdipmk import win_sdipmk, key_detect, nanosleep
from multiprocessing import Process, Array
from decimal import Decimal, ROUND_HALF_UP
from random import choice, uniform, gauss
from win32api import GetAsyncKeyState
from sys import executable, exit
from ctypes import windll
from time import sleep


if __name__ == '__main__':
    if not windll.shell32.IsUserAnAdmin():
        windll.shell32.ShellExecuteW(None, 'runas', executable, __file__, None, 1)
        exit(0)

    arr = Array('d', range(5))
    mou_detect_proc = Process(target=key_detect, args=(arr,))
    mou_detect_proc.name = bytearray(b'Code').decode('utf-8')
    mou_detect_proc.daemon = True
    mou_detect_proc.start()
    mouse_keybd = win_sdipmk()

    while not arr[2]:
        sleep(1)

    press_time = 90.0
    total_time = 120.0
    rand_range = 2.0
    VK_OEM_PLUS = 0xBB
    VK_OEM_MINUS = 0xBD
    print('按住侧键短促开火, 或者按F控制开关, 按END退出宏循环')
    print('Hold XButton to fire, or press "f" to switch on/off, press "END" to quit')
    print('左右调整总体间隔, 上下调整按住时间, 加减调整随机范围')
    print('Press left or right arrow to adjust total time interval, press up or down arrow to adjust hold time, press plus or minus key to adjust random range')

    预设炼狱 = [90.0, 120.0, 2.0]  # For Gatling Gun-Infernal Dragon
    预设USP = [55.0, 115.0, 5.0]  # For USP

    while not arr[0]:
        if GetAsyncKeyState(VK_F1)<0:
            press_time, total_time, rand_range = 预设炼狱

        if GetAsyncKeyState(VK_F2)<0:
            press_time, total_time, rand_range = 预设USP

        if GetAsyncKeyState(VK_LEFT)<0:  # 调整总体间隔
            total_time -= 0.5
            if total_time < 80:
                total_time = 80
        if GetAsyncKeyState(VK_RIGHT)<0:
            total_time += 0.5

        if GetAsyncKeyState(VK_DOWN)<0:  # 调整总体间隔
            press_time -= 0.25
            if press_time < 40:
                press_time = 40
        if GetAsyncKeyState(VK_UP)<0:
            press_time += 0.25

        if GetAsyncKeyState(VK_OEM_MINUS)<0:  # 调整随机范围
            rand_range -= 0.1
            if rand_range < 0:
                rand_range = 0
        if GetAsyncKeyState(VK_OEM_PLUS)<0:
            rand_range += 0.1
            if rand_range > 15:
                rand_range = 15

        rand_press = uniform(press_time-rand_range, press_time+rand_range)

        if arr[1]:
            mouse_keybd.function_p(1)
            nanosleep(rand_press)
            mouse_keybd.function_r(1)
            nanosleep(gauss(total_time, rand_range/2) - rand_press)
        else:
            nanosleep(100)

        print('开关(switch):{}, 总体间隔(interval):{}, 按住时间(hold):{}, 随机范围(random):{}'.format(arr[1]>0, Decimal(total_time).quantize(Decimal('1.00')), Decimal(press_time).quantize(Decimal('1.00')), Decimal(rand_range).quantize(Decimal('1.00'))), end='\r')

    sleep(1)
    mouse_keybd.function_r(1)
    mou_detect_proc.terminate()
