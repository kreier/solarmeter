# Solarmeter
Output power of a solar panel measured over weeks

## Setup

The initial setup from January 2020 requires a Laptop with Vernier software to measure just one data point. The circuit looks like this:

![load circuit for the solar panel and voltage measurement](pic/setup_2020-01-16.jpg)

For the future design of April 2020 the voltage is measured by the ESP32 and the value transfered to a database in the internet every 5 minutes. This gives 288 data points per day.

Additionally there are 4 digital switches for different loads planned. The ESP32 is activating them prior to the measurement and can combine the swithces for 16 different load values.



With the 

The data will be found at 

## Materials

We take a ESP32 for measuring and transmitting the data over Wifi.

Power from the solar cell, backup LiPo battery with a JST-PH 2.0 connector? What is the size from Adafruit Feather? Let's see and look for the power consumption as well ...

## Measurements and results

Data account is created, will be linked soon.

## History

> 2019/12/03

Interview with Tom about his EE project about renewable energy in Vietnam. As for an EE (extended essay in the IB international baccalaureate) in physics an experiment should be included. And we can investigate the wind and solar power here in Nha Be on the roof of the 5th floor of the school building.

To collect data I created a new Google account as aisvn.data for emails and communication (MQTT, IFTTT). 

> 2020/01/16

For about an hour during assigned study we tried to measure the characteristics of a 12 Volt 6 Watt solar panel that the school provided. The values are measured with the [Vernier voltage probe](https://www.vernier.com/product/voltage-probe/) and the [current probe](https://www.vernier.com/product/current-probe/). Unfortunately the voltage probe has a maximum voltage range of 10 Volt, while the solar panel provides up to 16 Volt without any load.

__Solution 1:__ A voltage divider made of two 1 kiloOhm resistor divided the output by 2 and moved it into the voltage limit of the probe. A successful reading now indicated the second problem: Without any load the output voltage is almost constant, since it mainly derives from the band gap in the semiconductor. This is well illustrated in this [graph from wikipedia](https://commons.wikimedia.org/wiki/File:Actual_output_in_volts,_amps,_and_wattage_from_a_100_Watt_Solar_module_in_August.jpg):

<img src="pic/hourly_production.jpg" align="center" width="80%">

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

To the code ...
