# RRR
Rewinch Rider Remote

wireless long range remote controller for rewinch

HowToUpdate RRR:

1. Install Arduino IDE: https://www.arduino.cc/en/software
2. Install heltec ESP32 Boards: https://heltec-automation-docs.readthedocs.io/en/latest/esp32/quick_start.html
  (short: Preferences->URL's->https://resource.heltec.cn/download/package_heltec_esp32_index.json // Tools->Boards->BoardManager->"heltec")
3. here (GitHub): Code -> Download ZIP
4. Install the following libraries (extract both rar's to: Documents/Arduino/libraries)
    
    - SolidGeek-VescUart-extended
    
    - Heltec_ESP32_Dev-Boards-extended
5. Extract and open the .ino's (IDE will create a folder for each) and upload* code to the Controller // Receiver
    
    - SND**.ino = Sender = Remote Controller
    
    - RCV**.ino = Receiver

- - - - - -

*before clicking upload button, make sure you got a microUSB-DATA-cable (there are a lot of "just-load" cables around) and check following settings (in Tools):

Board: "WiFi LoRa 32(V2)"

Upload Speed: "921600

CPU Frequency: "240MHz (WiFi/BT)"

Core Debug Level: "None"

PSRAM: "Disabled"

LoRaWan Region: "REGION_EU433"

LoRaWan Debug Level: "None"

Port: COMx (depends on which USB-Slot you are connected)
