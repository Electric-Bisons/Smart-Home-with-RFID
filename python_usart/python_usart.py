import serial
import time
import Tkinter
import threading

def HTML_SMART_HOME(data):
   f = open("data.txt","w")
   f.write(data)
   f.close()

#open serial port on COM4 and set baudrate at 9600
usart = serial.Serial('COM4')
usart.baudrate = 9600
print "Connected to: " + usart.portstr

#create variables to store remote control vectors
DEFAULT        = [0x00, 0x00, 0x00, 0x00]
CH_MINUS       = [0x00, 0xFF, 0xA2, 0x5D]
CH             = [0x00, 0xFF, 0x62, 0x9D]
CH_PLUS        = [0x00, 0xFF, 0xE2, 0x1D]
BACKWARD       = [0x00, 0xFF, 0x22, 0xDD]
FORWARD        = [0x00, 0xFF, 0x02, 0xFD]
PLAY_PAUSE     = [0x00, 0xFF, 0xC2, 0x3D]
VOL_MINUS      = [0x00, 0xFF, 0xE0, 0x1F]
VOL_PLUS       = [0x00, 0xFF, 0xA8, 0x57]
EQ             = [0x00, 0xFF, 0x90, 0x6F]
FOLDER_MINUS   = [0x00, 0xFF, 0x98, 0x67]
FOLDER_PLUS    = [0x00, 0xFF, 0xB0, 0x4F]
NUMBER_0       = [0x00, 0xFF, 0x68, 0x97]
NUMBER_1       = [0x00, 0xFF, 0x30, 0xCF]
NUMBER_2       = [0x00, 0xFF, 0x18, 0xE7]
NUMBER_3       = [0x00, 0xFF, 0x7A, 0x85]
NUMBER_4       = [0x00, 0xFF, 0x10, 0xEF]
NUMBER_5       = [0x00, 0xFF, 0x38, 0xC7]
NUMBER_6       = [0x00, 0xFF, 0x5A, 0xA5]
NUMBER_7       = [0x00, 0xFF, 0x42, 0xBD]
NUMBER_8       = [0x00, 0xFF, 0x4A, 0xB5]
NUMBER_9       = [0x00, 0xFF, 0x52, 0xAD]

#variables
temperature = 'Temperature = '
globalTemperature = '0'
temperatureIndex = 0

#funtion to send on serial specific vector
def buttonFunction(button):
   time.sleep(0.1)
   for i in button:
      time.sleep(0.1)
      usart.write(chr(i))
      time.sleep(0.1)
      #print the vector
      print i

#function to recieve data from serial
def serialRead():
   global temperature
   data = usart.read()
   #store the new data received
   temperature += data
   temperatureFunction()
   time.sleep(0.01)
   thread.run()

#function to print the temperature
def temperatureFunction():
   global temperatureIndex
   global temperature
   global globalTemperature
   #update index
   temperatureIndex += 1
   #store '.' char after two digits
   if(temperatureIndex == 2):
      temperature += '.'
   #print the temperature
   if(temperatureIndex > 2):
      temperatureIndex = 0
      globalTemperature = temperature + '&deg' + 'C'
      print globalTemperature
      #print on HTML the temperature
      HTML_SMART_HOME(globalTemperature)
      #reinitialize the register
      temperature = 'Temperature = '

#button text matrix
button_string =  [["CH-", "CH", "CH+"],
                  ["|<<", ">>|", ">||"],
                  ["VOL-", "VOL+", "EQ"],
                  ["0", "FOLDER-", "FOLDER+"],
                  ["1", "2", "3"],
                  ["4", "5", "6"],
                  ["7", "8", "9"]]

#button vectors matrix
button_action =  [[DEFAULT, DEFAULT, DEFAULT],
                  [BACKWARD, FORWARD, DEFAULT],
                  [VOL_MINUS, VOL_PLUS, EQ],
                  [NUMBER_0, FOLDER_MINUS, FOLDER_PLUS],
                  [NUMBER_1, NUMBER_2, NUMBER_3],
                  [NUMBER_4, NUMBER_5, NUMBER_6],
                  [NUMBER_7, NUMBER_8, NUMBER_9]]

#create a thread for the serial reading
thread = threading.Thread(target=serialRead)
thread.start()

#create buttons
frame = Tkinter.Tk()
frame.title = "Smart Home Remote Control"
for r in range(7):
   for c in range(3):
      #print the buttons vectors
      element = button_action[r][c]
      print element
      button = Tkinter.Button(frame, text=button_string[r][c],
                              height=5 , width=10,
                              command=lambda r=r, c=c: buttonFunction(button_action[r][c]))
      button.grid(row=r, column=c)

#run a mainloop for the GUI
frame.mainloop()
