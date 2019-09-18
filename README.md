# Doorlocking.App - An open source and secure way to open a door using wifi network. 

# In Action

See in those videos doorlocking.app in action <a href="https://www.youtube.com/watch?v=yd64wglDlZc" target="_blank">with</a> and  <a href="https://www.youtube.com/watch?v=D8Kzh0A3eeo" target="_blank">without</a> a box. Excited about the project? Follow the instructions below to deploy it by yourself! Please, [let me know](mailto:diogosmendonca@gmail.com) if you deployed doorlocking.app! Are you not technical? [Contact me](mailto:diogosmendonca@gmail.com) if you need help to deploy the system. 

# Hardware Setup

You will need the hardware bellow to deploy the system (Expected price of all hardware needed: US$20 - US$30):

1. An eletromagnetic door locking. Anyone that is triggered by a 12 volts eletric pulse. We used an [eletric strike](https://en.wikipedia.org/wiki/Electric_strike), but you can find many others [here](http://en.zsanxing.com). Expected price US$8.

2. NodeMCU v3 microcontroller (tested only with CP2102 chipset, we did not tested in Lolin). Expected price US$5.

3. A 400 Tie Points Protoboard. Expected price US$1.

4. Jumpers Kit. Female-Female, Female-Male, and Male-Male. Expected price US$2.

5. Protoboard Power Supply. In 12V, out 5V and 3.3V. Expected price US$1.

6. Power Supply 12V - 2A. Expected price US$2.

6. Relay module with one channel. Expected price US$1.

7. An WIFI router, you problably already have one. Expected price U$10, if needed. 

8. Assemble the hardware as instructed in [docs/](https://github.com/diogosmendonca/doorlocking/tree/master/docs) folder. 

# Steps to deploy software system

Instructions to deploy and testing the system. 

1. Open the file src/doorlocking/data/network.txt and fill ssid and password fields with the credentials of your wifi network. You may want to change hostname field as well, thus chose a name for your door and fill that field. 

2. Open the file src/doorlocking/data/users.txt and change the admin password. The default password is not secure. 

3. Generate an self signed SSL certificate using OpenSSL ([how to do it](https://stackoverflow.com/questions/10175812/how-to-create-a-self-signed-certificate-with-openssl?answertab=votes#tab-top)), and replace the content of the certificates in /src/doorlocking/doorklocking.ino with the content of .crt and .key created by OpenSSL. 

4. Flash the firmeware in the NodeMCU using Arduino IDE.

5. Using Arduino IDE and Write the dorlocking/data files to the NodeMCU SPIFF file system. See how to [here](http://arduino.esp8266.com/Arduino/versions/2.0.0/doc/filesystem.html).

6. If your wifi network uses DHCP, identify the IP address that wifi router assigned to your microcontroller. A message will be printed in the serial port monitor, if your are using it. Other option is to check the address assigned in your router admin interface. 

7. Access the address of the microcontroller using HTTPS prefix. Example: https://192.168.10.200/

8. Use the credentials of admin, as you setup in step 2, to perform login.

---
# Development Cycle Steps

1. If you need to change the web app use the grunt command bellow to minify and prepare the source code to be written in the microcontroller.  

```
grunt build --force
```

2. Flash the firmeware in the NodeMCU using Arduino IDE. 

3. Using Arduino IDE and Write the dorlocking/data files to the NodeMCU SPIFF file system. See how to [here](http://arduino.esp8266.com/Arduino/versions/2.0.0/doc/filesystem.html).

