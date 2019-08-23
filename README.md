# doorlocking

Instructions to deploy and testing the system. 

1. Build web app. In the directory app run:

```
grunt build --force
```

2. Using Arduino IDE and Write the dorlocking/data files to the NodeMCU SPIFF file system. See how to [here](http://arduino.esp8266.com/Arduino/versions/2.0.0/doc/filesystem.html).

3. Flash the firmeware in the NodeMCU using Arduino IDE.

4. Connect to the new wi-fi created using connection data setup in data/config_network.txt file.

5. Access https://192.168.1.1/

6. Use the credentials contained in data/users.txt file to log in.

