# Embedded-Systems---Sensors
Wi-Fi Module, RFID, Color, Touch, Proximity and Ultrasound Sensor (ATM e-Surveillance)

1) Distance measurement with Ultrasonic Sensor - _ATM Surveillance for Shutter Door_

This project was for measuring the distance of ATM Shutter to indicate the activity occuring iside the ATM.
-   Shutter is open: ATM Vestibule accessible to civillans
-   Shutter midway: Loading/Unloading ATM with cash
-   Shutter closed: Maintenance work at the site

The Ultrasonic Sensor emits a burst of signal in every few seconds and is reflected from the object. The distance is then calculated based on duration o time and speed of sound. Th max. distance that HC-SR04 can measure is 400 cm. The measurement is displayed on serial terminal. 

![](https://github.com/aayushi-95/Embedded-Systems-Sensors/blob/master/images/Capt1ure.PNG) ![](https://github.com/aayushi-95/Embedded-Systems-Sensors/blob/master/images/Picture2.png)

2) ESP Module - _Interfaced with Keypads, Relay to control ATM Front and Back Doors_

ESP8266 is an SoC Wi-Fi Device used for IoT Platforms. Can be programmed on different compilers : Arduino, ESPlorer, Linux. In this project ESP8266 was integrated to various boards to suit multi-purposes: keypads, relay boards, NONC (ATM producs). It follows TCP/IP Protocol but can use Telnet, UDP.
With 16 GPIO pins, communication via I2C, SPI and UART and 10 bit ADC it is compatibe with many circuits. All the prodcuts are connected to a local network created by this Wi-Fi module and request BeagleBone Black which acts as a server to serve the company website and unique IP address is assigned to each board.

![](https://github.com/aayushi-95/Embedded-Systems-Sensors/blob/master/images/Picture4.png) ![](https://github.com/aayushi-95/Embedded-Systems-Sensors/blob/master/images/Picture5.png)

3) Touch Sensor - _Kiosk Product_

This sensor was a part of LifeStyle kisok Project for enticing more customers. The idea behind this board is that the customer can touch the like button and a touch sensor behind the dial will register the count and send command to BeagleBone Black interfaced with a relay circuit to monitor or increase the number.

![](https://github.com/aayushi-95/Embedded-Systems-Sensors/blob/master/images/Picture3.png)

4) Color Sensor - _SmartBin_

Battery operated Smart bin had a color sensor implanted TCS230 to check the toxicity of waste inside. It was designed to use in hospitals to check the biohazard substances disposed and alert the system based on predefined colors. PWM of PIC16F was exploited to serve the purpose. It would capture and measure teh frequency to detect the color of disposed waste.

![](https://github.com/aayushi-95/Embedded-Systems-Sensors/blob/master/images/Picture1.png)
