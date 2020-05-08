# Solarmeter

This project continuously measures the output power of a solar cell. The data is collected in Ho Chi Minh City 2020 and part of an EE (extended essay) in IB Physics at the AISVN.

## Setup

The initial setup from December 2019 requires a Laptop with Vernier software to measure just one data point. The circuit looks like this:

![load circuit for the solar panel and voltage measurement](pic/setup_2020-01-16.jpg)

For the future design of April 2020 the voltage is measured by the ESP32 and the value transfered to a database in the internet every 5 minutes. This gives 288 data points per day. An article at [randomnerdtutorials](https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/) explains the setup and programming very well;

![Setup of ESP32](pic/analog_input_esp32.jpg)

Additionally there are 4 digital switches for different loads planned. The ESP32 is activating them prior to the measurement and can combine the swithces for 16 different load values. The setup now looks like this:

![adjustable load setup ESP32](pic/setup_2020-03-20.jpg)

With the 4 switches we can create 16 datapoints, that the ESP32 can read in 12 bit. Every 5 minutes we create therefore 24 byte of data. Over a day this accumulates to 6912 byte and in a year all data collected is 2.5 MByte.



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

First we had to estimate the correct resistance for the load. With the parameters of 12V and 6W one can calculate a current of I=500mA from __P=VI__ and then apply Ohm's law as __R=U/I__ which results in a resitance of R=12/0.5=24 Ohm. Another way is the direct way of __R=U/P=12/6=24 Ω__. Half the power would be achieved with ca. 50 Ohm and 100 Ohm creates 25% of the maximum power on the load. Since we had plenty 1 kOhm resistors we soldered 9 of them in parallel and added 2 series of two 1 kOhm resistors from the voltage divider as well. The circuid diagram looks like this:

![load circuit for the solar panel and voltage measurement](pic/setup_2020-01-16.jpg)

> 2020/02/27
<img src="pic/hybrid.jpg" width="30%" align="right">

The hybrid wind solar power generator arrives - despite the corona virus outbreak in China. Declarations with customs and DHL took some time, but it's now here. Other parts will be ordered locally. And this controller has the __MPPT__ we mentioned in January included!

> 2020/03/18

I finally create a Github repository to document this project. The ESP32 were delivered some weeks ago. Now I got the LiPo batteries as well. Connector for the feather-style boards: __JST PH 2pin__ for reference! Purcheased at [ICDAYROI](https://icdayroi.com/) in Thủ Đức.

> 2020/03/20

The ADC of the ESP32 is not very linear. But we want to use it to measure the voltage of the solar pannel under different load situations. There might be a compensation function. The procedure and measurement was done by [Fernando Koyanagi](https://www.fernandok.com/) from Florianópolis in Brazil and published in [instructables](https://www.instructables.com/id/Professionals-Know-This/).

![ADC reading](pic/adc_esp32.jpg)

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
- Hibernate: 1.3 mW, runtime 2857 hours - or 119 days
