# Solarmeter

This project continuously measures the output power of a solar cell. The data is collected in Ho Chi Minh City 2020 and part of an EE (extended essay) in IB Physics at the AISVN. Our first result from AISVN with the solar panel looks like this:

![voltage over a day](aisvn/data/data_2020-05-21.jpg)

![battery week](aisvn/data/2020-05-26_battery_week.jpg)

![voltage in phu my hung](pic/May2020_6V_2W_panel.jpg)

![power in phu my hung](pic/May2020_power.jpg)

## Setup

The initial setup from December 2019 requires a Laptop with Vernier software to measure just one data point. The circuit looks like this:

![load circuit for the solar panel and voltage measurement](pic/setup_2020-01-16.jpg)

This is the setup in June 2020:

**still to come**





## Materials

<img src="pic/TTGO_ESP32.jpg" width="30%" align="right">

We take a ESP32 for measuring and transmitting the data over Wifi.

Power from the solar cell, backup LiPo battery with a JST-PH 2.0 connector? What is the size from Adafruit Feather? Let's see and look for the power consumption as well ...

I ordered some setup materials:

- [Solar panel 12V 3W 145x145mm](https://www.thegioiic.com/products/tam-nang-luong-mat-troi-3w-12v) enough for ESP32?
- [Solar panel 12V 1.5W 115x85mm](https://www.thegioiic.com/products/tam-nang-luong-mat-troi-1-5w-12v) I guess that's what Tom uses
- [Solar panel 5V .25W 45x45mm](https://www.thegioiic.com/products/tam-nang-luong-mat-troi-0-25w-5v) small and cheap - sufficient? Without step down converter?
- [Step down to 5V](https://www.thegioiic.com/products/mach-giam-ap-usb-ra-5v3a)
- [ESP32 with display, USB-C](https://www.lazada.vn/products/i325250821.html) 222000 VND - $ 9.40 with 1.14" display
- Battery 500 mAh for ...

## Software

``` c
/*
  ReadAnalogVoltage

  Reads an analog input on pin 0, converts it to voltage, and prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ReadAnalogVoltage
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  Serial.println(voltage);
}
```

## Measurements and results

Data account is created, will be linked soon. Mostly to be found at [sites.google.com/ais.edu.vn/solar](https://sites.google.com/ais.edu.vn/solar).

## History

> 2019/12/03

Interview with Tom about his EE project about renewable energy in Vietnam. As for an EE (extended essay in the IB international baccalaureate) in physics an experiment should be included. And we can investigate the wind and solar power here in Nha Be on the roof of the 5th floor of the school building.

To collect data I created a new Google account as aisvn.data for emails and communication (MQTT, IFTTT). 

> 2019/12/08

The website [sites.google.com/ais.edu.vn/solar](https://sites.google.com/ais.edu.vn/solar) is created. The measured data from the last 36 hours is displayed in an interactive graph.

> 2019/12/14

I created a jupyter notebook with the first informations. The document can be found [here](https://colab.research.google.com/drive/1SWBxNhv9skyehvX9P8T1SB_Elqk8s_KK?usp=sharing). A copy is included in software.

> 2019/12/17

For about an hour during assigned study we tried to measure the characteristics of a 12 Volt 6 Watt solar panel that the school provided. The values are measured with the [Vernier voltage probe](https://www.vernier.com/product/voltage-probe/) and the [current probe](https://www.vernier.com/product/current-probe/). Unfortunately the voltage probe has a maximum voltage range of 10 Volt, while the solar panel provides up to 16 Volt without any load.

__Solution 1:__ A voltage divider made of two 1 kiloOhm resistor divided the output by 2 and moved it into the voltage limit of the probe. A successful reading now indicated the second problem: Without any load the output voltage is almost constant, since it mainly derives from the band gap in the semiconductor. This is well illustrated in this [graph from wikipedia](https://commons.wikimedia.org/wiki/File:Actual_output_in_volts,_amps,_and_wattage_from_a_100_Watt_Solar_module_in_August.jpg):

<p align="center">
<img src="pic/hourly_production.jpg" width="80%">
</p>

Note that only the current increases during daytime. Since the power is a product of voltage and current, the power increases as well.

__Solution 2:__ A [MPPT](https://en.wikipedia.org/wiki/Maximum_power_point_tracking) (Maximum Power Point Tracking) involves a lot electronics that is well beyond the scope of IB physics. We chose to use a fixed load somewhere in the middle of the power curve. By measuring the voltage we would automatically measure the current and therefore the power.

First we had to estimate the correct resistance for the load. With the parameters of 12V and 6W one can calculate a current of I=500mA from __P=VI__ and then apply Ohm's law as __R=U/I__ which results in a resitance of R=12/0.5=24 Ohm. Another way is the direct way of __R=U²/P=12²/6=24 Ω__. Half the power would be achieved with ca. 50 Ohm and 100 Ohm creates 25% of the maximum power on the load. Since we had plenty 1 kOhm resistors we soldered 9 of them in parallel and added 2 series of two 1 kOhm resistors from the voltage divider as well. The circuid diagram looks like this:

![load circuit for the solar panel and voltage measurement](pic/setup_2020-01-16.jpg)

> 2020/02/27
<img src="pic/hybrid.jpg" width="30%" align="right">

The hybrid wind solar power generator arrives - despite the corona virus outbreak in China. Declarations with customs and DHL took some time, but it's now here. Other parts will be ordered locally. And this controller has the __MPPT__ we mentioned in January included!

> 2020/03/18

I finally create a Github repository to document this project. The ESP32 were delivered some weeks ago. Now I got the LiPo batteries as well. Connector for the feather-style boards: __JST PH 2pin__ for reference! Purcheased at [ICDAYROI](https://icdayroi.com/) in Thủ Đức.

> 2020/03/20

The ADC of the ESP32 is not very linear. But we want to use it to measure the voltage of the solar pannel under different load situations. There might be a compensation function. The procedure and measurement was done by [Fernando Koyanagi](https://www.fernandok.com/) from Florianópolis in Brazil and published in [instructables](https://www.instructables.com/id/Professionals-Know-This/).

![ADC reading](pic/adc_esp32.jpg)

For the future design of April 2020 the voltage is measured by the ESP32 and the value transfered to a database in the internet every 5 minutes. This gives 288 data points per day. An article at [randomnerdtutorials](https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/) explains the setup and programming very well;

![Setup of ESP32](pic/analog_input_esp32.jpg)

Additionally there are 4 digital switches for different loads planned. The ESP32 is activating them prior to the measurement and can combine the swithces for 16 different load values. The setup now looks like this:

![adjustable load setup ESP32](pic/setup_2020-03-20.jpg)

With the 4 switches we can create 16 datapoints, that the ESP32 can read in 12 bit. Every 5 minutes we create therefore 24 byte of data. Over a day this accumulates to 6912 byte and in a year all data collected is 2.5 MByte.

> 2020/04/01

The ordered TTGO ESP32 mainboard is pretty good! I ordered it mainly for the included LiPo charger, but it as 2 extra buttons (GPIO0 and GPIO 35) to the reset button. And a [1.14 Inch display](http://www.lcdwiki.com/1.14inch_IPS_Module) IPS [ST7789V](https://www.newhavendisplay.com/appnotes/datasheets/LCDs/ST7789V.pdf) with 135x240 pixel. And there are 13 GPIO pins left to use for 4 switches and one voltmeter under different load conditions.

![Pin layout TTGO T-Display V1.0](pic/TTGO_pin.jpg)

To program the display and some voltages, example code can be found at [https://github.com/Xinyuan-LilyGO/TTGO-T-Display](https://github.com/Xinyuan-LilyGO/TTGO-T-Display)

> 2020/04/07

A second TTGO ESP32 arrived and is set to be programmed:

![Two ESP32 TTGO](pic/ttgo.jpg)

Without programming TTGO installed some software on the module. On the 135x240 display you get 22x30 characters with a 6x8 font. That's almost as much as my first ZX81 with 32x24. You can scan for nearby 2.4GHz networks, check the power supply and go into sleep mode. The last one is of interest for our project and the power consumption with these standard settings will be tested next.


![TTGO-T-Display modes](pic/ttgo_disp1.jpg)

> 2020/04/28

With an external power supply we can check the power consumption of the TTGO. The values over USB were rather high, but it has a 3.7 LiPo battery connector and the power consumption there is significantly lower, most likely because the 5V to 3.3V step-down converter is not needed. Here are the values for 3.7 Volt over battery:

- Power on, start up and screen on: 68 mA
- WiFi scan for nearby hotspots: 108 mA
- Sleep, waiting for interrupt from key pressed: 0.35 mA

With a 1000 mAh battery and the given voltage we can calculate the power consumption and projected runtime:

- Screen on: 252 mW, runtime 14 hours 42 minutes
- WiFi on: 399 mW, runtime 9 hours 15 minutes
- Hibernate: __1.3 mW__, runtime 2857 hours - or __119 days__

> 2020/05/07

School is back open since May 4th, students are back since May 5th - and now we got the solar panel and the battery! Time to find a place on the roof in Nha Be and control software to collect and transmit data.

![Solar panel](pic/2020-05-07_solar.jpg)

> 2020/05/08

The wind generator arrived just one day later! Looked at location on top of the roof, 6th floor in Nha Be. Empty room for equipment is there, rain proved, and space for the 5 wired from solar and wind to the control unit. Maybe next week start first test setup?

![Wind generator](pic/2020-05-08_wind.jpg)

Data: __JLS-500__ with 24V and 500 W

> 2020/05/14

The TTGO can be programmed with MicroPython and [devbis](https://github.com/devbis) created both a [slow python driver](https://github.com/devbis/st7789py_mpy/) for the display ST7789 as well as a [fast C variant](https://github.com/devbis/st7789_mpy). Further description of this module [here](https://sites.google.com/site/jmaathuis/arduino/lilygo-ttgo-t-display-esp32). And the tft driver from Loboris is working as well, details in [this instuctable from February 2020](https://www.instructables.com/id/TTGO-color-Display-With-Micropython-TTGO-T-display/). Original data at [LilyGo github](https://github.com/Xinyuan-LilyGO/TTGO-T-Display).

> 2020/05/15

We installed the 60W solar module on the roof of our school AISVN and connected MPPT controller and __24Ah__ battery. Now charging over the weekend, then connect my 60W motorcycle lamp to drain the battery every night ...

![solar installation 2020/05/15](pic/2020-05-15_solar.jpg)

The stand for the 600W wind generator will be welded in the next week.

> 2020/05/17

First successful setup with WEMOS LoLin32 board, 2000 mAh battery and two 1kOhm voltage divider for input on pin 34. Voltage measurement every 2 minutes, upload via __IFTTT__ and webhooks to google sheet, then deep sleep. Setup:

![Little solar setup](pic/2020-05-16_setup.jpg)

Code:

``` c
// Solarmeter first attempt (all Serial.print removed), inspired by 
// https://randomnerdtutorials.com/esp32-esp8266-publish-sensor-readings-to-google-sheets/
 
#include <WiFi.h>
#include <Wire.h>

const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
const char* resource = "/trigger/value/with/key/create-one";
const char* server = "maker.ifttt.com";

uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
uint64_t TIME_TO_SLEEP = 120;
int adcValue = 0;

void setup() {
  delay(1000);
  initWifi();
  makeIFTTTRequest();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);    
  esp_deep_sleep_start(); // start deep sleep for 120 seconds (2 minutes)
}

void loop() {  // sleeping so wont get here
}

void initWifi() {  // Establish a Wi-Fi connection with your router
  WiFi.begin(ssid, password);  
  int timeout = 10 * 4; // 10 seconds
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
  }
}

void makeIFTTTRequest() {  // Make an HTTP request to the IFTTT web service
  WiFiClient client;
  int retries = 5;

  // raw and converted voltage reading
  adcValue = analogRead( 34 );
  String jsonObject = String("{\"value1\":\"") + adcValue + "\",\"value2\":\"" 
    + (adcValue * 2.4)   + "\",\"value3\":\"" + millis() + "\"}";
  
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
        
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  client.stop(); 
}

```

And for the first day in Phy My Hung (May 17th, 2020) we got this graph:

![Voltage output during the day](data/2020-05-17_voltage.jpg)

Power measurements with the WEMOS LoLin32 lite:

- On 40 mA, regardless of LED on pin 22, 148 mW
- WiFi 116 mA fluctuating (DHCP, http request), 430 mW, full cycle < 1 second (0.7 s average)
- Sleep 0.06 mA, __0.22 mW__ - runtime with 1000 mAh battery: 16666 hours, or __694 days__

Power measurements with LilyGo TTGO T-Koala with WROVER-B and USB-C

- On 35.8 mA, power LED is always on, 133 mW
- WiFi 100 mA (spikes in oscilloscope, DHCP, http request), 370 mW, full cycle < 1 second (0.7 s average)
- Sleep 0.79 mA, 2.9 mW - runtime with 1000 mAh battery: 1265 hours, or 53 days, power LED is ON 

### Power output of 6V 2W solar panel on 39 Ohm resistor

> 2020/05/18

Here is the graph: 

![Voltage output during the second day](data/2020-05-18_voltage.jpg)

> 2020/05/19

With a load of 3 kOhm we get almost the free floating voltage of the solar cell. At 7 Volt the test circuit only consumes 2.3 mA or 16 mW - while having a maximum power of 2000 mW - this is 0.8% of what it should be able to deliver. Thats the output over the day:

![Voltage output during the third day](data/2020-05-19_voltage.jpg)
![Voltage output during the 4th day](data/2020-05-20_voltage.jpg)

> 2020/05/21

We installed the second ESP32 next to our MPPT Solar controller and measure for the beginning the voltage of the solar panel and the battery. Observations: The MPPT does not apply for the solar panel, it is directly connected to the battery and looses therefore a lot of energy. Secondly: The floating limit for our battery was set too low at 13.8 Volt, so it was never really charged since installation on May 15th. New limit is 14.7 Volt floating and overvoltage limit 14.8 Volt. Third - the lower voltage limit was set too low at 10.8 Volt, it is adjusted now at 11.0 Volt. The load was reduced from 120 W to 60 W. Let's see if the weekend brings an improvement. Here is the data from the first day (second half of May 21st, 2020):

![values and voltage battery from first day](pic/2020-05-21_nhabe.jpg)

### Peak voltage and power output of 6 Volt 2 Watt solar panel

> 2020/05/25

I combined several measurements from May 18th to May 24th with different loads on the 6 Volt 2 Watt solar panel that was placed in Phu My Hung to an overview from 5:00 AM tp 7:00 PM:

![voltage in phu my hung](pic/May2020_6V_2W_panel.jpg)

![power in phu my hung](pic/May2020_power.jpg)

### Results from first week at AISVN

> 2020/05/28

We start with a 60 Watt drain resistor that switches on after sunset (6:00 PM) for a programmed time of 4 hours. Unfortunately it drains the battery in 3.5 hours.

![May 21st](aisvn/data/2020-05-21_all.jpg)
![May 22nd](aisvn/data/2020-05-22_all.jpg)

Rainy Saturday, battery charged very little, drained after 1.5 hours.
![May 23rd](aisvn/data/2020-05-23_all.jpg)
![May 24th](aisvn/data/2020-05-24_all.jpg)

Switch from a 60 Watt load to a 5 Watt load for 4 hours after sunset.
![May 25th](aisvn/data/2020-05-25_all.jpg)

Battery fully charged, controller disconnects solar panel during the day. Load now on for 7 hours after sunset.
![May 26th](aisvn/data/2020-05-26_all.jpg)

Battery drained again with 60 Watt load from 11:00 AM to 5:00 PM. Then installed new MPPT solar charger.
![May 27th](aisvn/data/2020-05-27_all.jpg)

With MPPT higher solar voltage for charging - for increased efficiency. Compare May 28th to May 25th.
![May 28th](aisvn/data/2020-05-28_all.jpg)
![May 29th](aisvn/data/2020-05-29_all.jpg)

Graduation for Seniors 2020 started 3:00 PM after the thunderstorm 1:00 PM - solar panel went off.
![May 30th](aisvn/data/2020-05-30_all.jpg)
![May 31th](aisvn/data/2020-05-31_all.jpg)


### Measure the power - resistor for current in line

> 2020/06/01

We put a 0.1 Ohm resistor in series with  the positive wire of the solar panel to determine the current from the voltage drop. Even in the case of 3 Ampere this accounts only to 0.3 Volt and less than a Watt of heat loss.

![circuit diagram](pic/2020-06-01_powerbox.jpg)

> 2020/06/02

The powerbox was finished. The total resistance is closer to 0.122 Ohm. The voltage drop reading has therefore to be multiplied by 8.2 to get the current reading for the solar panel input. That's the finished box:

![finished box](pic/2020-06-02_powerbox.jpeg)

> 2020/06/05

The T-Koala has some direct pins, for example for the battery - it's easier to connect to them to determine the voltage of the LiPo. Picture and pinout:

![T-Koala from TTGO](pic/T-Koala.jpg)

And I finished the soldering of the board with several voltage dividers and two LEDs to lower the voltage into the measurable range of the ESP32 to see the voltage difference over the 0.1 Ohm resistor in line with the solar panel.

![board v0.1](pic/2020-06-05_board.jpg)

And I installed it after sunset to get the first measurements. It's installed parallel to the other ESP32 that operated just with 2 voltage dividers for the last 2 weeks - which will cause weired values because of wrong reference levels - and pin26 of the ESP.

![board v0.1](pic/2020-06-05_setup.jpg)
