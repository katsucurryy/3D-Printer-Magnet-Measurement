# -----------------------------モジュール------------------------------------------#

import csv  # CSVファイルの読み書き用ライブラリ
import datetime
from datetime import date
import dateutil.utils
import lakeshore
import matplotlib  # グラフ描画用ライブラリ
import matplotlib.pyplot as plt  # mataplotlibを利用するために必要，matplotlib.pyplotクラスをインポートし，慣例的に「plt」という別名をつける．
import os  # Pythonのコード上でOSに関する操作を可能にするモジュール
import sys  # Pythonのインタプリタや実行環境に関する情報を扱うためのライブラリ
import numpy as np  # ベクトルや行列といった数値計算するためのライブラリ
import serial  # シリアル通信を可能にするモジュール
from numpy import *  # numpyモジュール内で定義されているメソッドや変数をまとめてインポートできる．
from scipy.integrate import quad  # 指定された下限と上限の間の特定の関数の統合を評価するモジュール
import time  # 時間を扱うためのモジュール
import pyvisa as visa  # 計測器との通信を可能にするモジュール
import datetime  # 基本的な日付と時間に関するライブラリ
from pyvisa.resources import MessageBasedResource  # 計測器のpythonのクラスを指定するモジュール
import scipy.io as sio  # データ入出力を可能にするモジュール
import subprocess  # 外部ファイルを操作などのコマンド実行をできるモジュール
from openpyxl import Workbook  # Excelファイルを読み書きするためのモジュール
from lakeshore import Teslameter  # テスラメータを扱うためのライブラリ
import pandas as pd  # データ解析を支援するためのライブラリ
import pyperclip  # クリップボードの情報をコピーやペーストするためのモジュール

# -----------------------------フォルダ作成----------------------------------------#

"""now = datetime.datetime.now()
current_time = now.strftime("%Y-%m-%d-%H-%M")
dir_for_output = "./測定結果/"+current_time

os.makedirs(dir_for_output,exist_ok=True)
"""

"""def Xreset(x):
    for i in range(x):
        order = b'r'
        ser.write(order)
        responce = ser.read()
        if(responce == b'e'):
            break

def Yreset(y):
    for i in range(y):
        order = b'b'
        ser.write(order)
        responce = ser.read()
        if(responce == b'e'):
            break
"""
# ----------------------テスラメータ（Lake Shore）設定-----------------------------#

my_teslameter = Teslameter()  # テスラメータをUSBで接続
my_teslameter.command('SENSE:MODE DC')  # テスラメータがDCモードになるように設定
probe_serial_number = my_teslameter.query('PROBE:SNUMBER?')  # プローブのシリアルナンバーを問い合わせ
probe_temperature = my_teslameter.query('FETCH:TEMPERATURE?')  # プローブの温度を問い合わせ

with open("teslameter_data.csv", "w", newline="") as file:  # データを書き込むファイルを作成
    writer = csv.writer(file)

    file.write('Header Information\n')
    file.write('Instrument serial number:' + my_teslameter.serial_number + '\n')  # 測定器のシリアル番号を書き込み
    file.write('Probe serial number:' + probe_serial_number + '\n')  # プローブのシリアル番号を書き込み
    file.write('Probe temperature:' + probe_temperature + '\n')  # プローブの温度を書き込み
    file.write('Date:' + datetime.date.today().__str__() + '\n\n')  #current date and time

    header = ('Btotal', 'Bx', 'By', 'Bz')  # ヘッダー情報の書き込み
    writer.writerow(header)
    print(header)

# ------------------------------arduinoシリアル通信設定-------------------------------------#

arduino = serial.Serial(port='COM4', baudrate=9600, dsrdtr=False, rtscts=False, timeout=2)


# -------------------------3Dプリンタの稼働距離・範囲設定----------------------------#

length_x=20 #mm area creation
length_y=60

rows=30
columns=10

MeasX = columns
MeasY = rows
totm=MeasX*MeasY
space=length_y/MeasY
print(rows," rows and ",columns," columns for a total of ",totm," measurements with a spacing of ",space,"mm")
print('Date: ' + datetime.date.today().__str__() + '\n\n')

# -------------------------3Dプリンタでのtimer------------------------------#
def time_convert(sec):
    mins = sec//60
    sec= sec%60
    hours =mins//60
    mins=mins%60
    print("time lapsed = {0}:{1}:{2}".format(int(hours),int(mins),sec))


# -------------------------3Dプリンタでのtimer------------------------------#
#this next code is very important the bed is 240 set starting postion by 240-x for 150mm at center 240-45 is 195 set x and y measurements
test = "csx110x020mx010y060my030sy090"
tt=""
r="r"
"""
for i in range(5):
  arduino.write(test.encode())
  print(arduino.readline())
  time.sleep(2)
ts = datetime.datetime.now()
"""

arduino.write(test.encode())
print(arduino.readline())
arduino.write(test.encode())
print(arduino.readline())
print(arduino.readline())


time.sleep(20)

w="f"
for y in range(int(MeasX)):  # 3Dプリンタの制御
 arduino.write(r.encode())
 print("reset")
 time.sleep(.2)
 yy=y+1
 if((y%2)==0):
     if (y >= 10):
         w = "f" + str(y)
     if (y < 10):
         w = "f0" + str(y)
 if((y%2)!=0):
     if (y >= 10):
         w = "f" + str(y)
     if (y < 10):
         w = "f0" + str(y)

 print("collection #"+str(y))




 for x in range(int(MeasY)):

     start_time = time.time() #for measurement speed
     with open("teslameter_data.csv", "a", newline="") as file:
         tt=w
         xx = x + 1
         if ((x % 2) == 0):
             if (x >= 10):
                 w = w + str(x)
             if (x < 10):
                 w = w + "0" + str(x)
         if ((x % 2) != 0):
             if (x >= 10):
                 w = w + str(x)
             if (x < 10):
                 w = w+ "0" + str(x)

         print(w)
         arduino.write(w.encode())
         if (x==0):

            time.sleep(10)
            print("sleepover")

         time.sleep(1.2)

         for h in range(5):

             writer = csv.writer(file)
             field = my_teslameter.get_dc_field_xyz()  # 測定値を取得
             result = field
             print(result)
             writer.writerow(result)


           # csvに書き込み

         print(time_convert(time.time()-start_time))

         print(arduino.readline())
         w=tt


sys.exit()
