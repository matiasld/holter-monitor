# holter-monitor
Full heart Holter Monitor   
  ![alt text](https://github.com/matiasld/holter-monitor/blob/master/img/flowchart.png?raw=true)
  

Sensors  
Heart rate data can be really useful whether you're designing an exercise routine, studying your activity or anxiety levels or just want your shirt to blink with your heart beat. 
The pulse sensor used in this project is a plug-and-play heart-rate sensor for Arduino. It can be used by students, artists, athletes, makers, and game & mobile developers who want to easily incorporate live heart-rate data into their projects. It essentially combines a simple optical heart rate sensor with amplification and noise cancellation circuitry making it fast and easy to get reliable pulse readings. Also, it sips power with just 4mA current draw at 5V so it's great for mobile applications.  
  
![alt text](https://github.com/matiasld/holter-monitor/blob/master/img/sensor.jpg?raw=true)

Simply clip the Pulse Sensor to your earlobe or finger tip, infrared sensor reads blood pulse by comparing emitted and received light variations.  
  
Microcontroller  
The microcontroller used in this project is LPC1769, an ARM Cortex M3 by NXP; to sample heart pulse sensors by using its own ADC and, if usb connection available, sending data to PC application or showing waveforms in a TFT Display and saving in a local SD* storage via SPI protocol by default. It has to be fast enough to capture QRS heart waveform of the ECG (Electrocardiogram) to read effective data.  
  
  ![alt text](https://github.com/matiasld/holter-monitor/blob/master/img/ECG.png?raw=true)
  
More about ECG: http://en.wikipedia.org/wiki/Electrocardiography  
*SD: Fat32 format  
  
PC  
The pc application was made with Qt Creator 4 to communicate via USB, if available, with the monitoring device (uController) allowing us to see a more detailed version of waveforms and make local PC recordings.  
  
  ![alt text](https://github.com/matiasld/holter-monitor/blob/master/img/qt2.png?raw=true)
