# Solarmeter
Output power of a solar panel measured over weeks

## Setup

sketch follows

## Materials

We take a ESP32 for measuring and transmitting the data over Wifi.

Power from the solar cell, backup LiPo battery with a JST-PH 2.0 connector? What is the size from Adafruit Feather? Let's see and look for the power consumption as well ...

## Measurements and results

Data account is created, will be linked soon.

## History

> 2019/12/03

Interview with Tom about his EE project about renewable energy in Vietnam. As for an EE (extended essay in the IB international baccalaureate) in physics an experiment should be included. And we can investigate the wind and solar power here in Nha Be on the roof of the 5th floor of the school building.

To collect data I created a new Google account as aisvn.data for emails and communication (MQTT, IFTTT). 

> 2020/02/27
<img src="pic/hybrid.jpg" width="30%" align="right">

The hybrid wind solar power generator arrives - despite the corona virus outbreak in China. Declarations with customs and DHL took some time, but it's now here. Other parts will be ordered locally.

> 2020/03/18

I finally create a Github repository to document this project. The ESP32 were delivered some weeks ago. Now I got the LiPo batteries as well. Connector for the feather-style boards: __JST PH 2pin__ for reference! Purcheased at [ICDAYROI](https://icdayroi.com/) in Thủ Đức.

> 2020/03/20

The ADC of the ESP32 is not very linear. But we want to use it to measure the voltage of the solar pannel under different load situations. There might be a compensation function. The procedure and measurement was done by [Fernando Koyanagi](https://www.fernandok.com/) from Florianópolis in Brazil and published in [instructables](https://www.instructables.com/id/Professionals-Know-This/).

![ADC reading](pic/adc_esp32.jpg)

To the code ...
