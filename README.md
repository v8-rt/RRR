# RRR
Rewinch Rider Remote

wireless long range remote controller for rewinch

HowToUpdate RRR:

1. Install Arduino IDE: https://www.arduino.cc/en/software
2. Install heltec ESP32 Boards: https://heltec-automation-docs.readthedocs.io/en/latest/esp32/quick_start.html
  (short: Preferences->URL's->https://resource.heltec.cn/download/package_heltec_esp32_index.json // Tools->Boards->BoardManager->"heltec")
3. Install necessary libraries:
    -SolidGeek-VescUart-extended
    -Heltec_ESP32_Dev-Boards-extended
4. Open .ino's and upload code to the Controller // Receiver
    -SND**.ino = Sender = Remote Controller
    -RCV**.ino = Receiver
