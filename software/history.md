Version history for different measurement stations.

## SolarAISVN

v0.9 20200617
- transmitted data moved from millivolt to volt and milliAmpere to Ampere
- Transmission of power value included
- WiFi credentials exported to credentials.h

v0.8 20200615 
- no more negative voltages, converted to zero
- offset for Solar panel #2 corrected

v0.7 20200615
- more voltage conversions in code
- submission expanded to 2 minutes again

v0.6 20200610
- all voltages calibrated
- voltage from __wind generator__ is now measured

v0.5 20200608
- hack reg_b register SENS_SAR_READ_CTRL2_REG to reactivate ADC2 after deepsleep and WiFi activation

v0.4 20200607
- multisampling 100x removes noise, but increases measured voltages by 25 mV

v0.3 20200605
- weekend test version, measures every minute
- __included current__ measurement by voltage difference over 0.122 Ohm resistor

v0.2 20200519
- pin34 raw measurement, __solar and battery voltage__

v0.1 20200516
- initial release
- only measures voltage solar panel

## SolarAISVN2

v0.1 20200618
- Initial release
- Measures voltage solar panel #3, battery #2 voltage, battery #2 current, LiPo2 voltage

## SolarPHUMY

v0.1 2020
- based on T-Koala like the SolarAISVN
- upload with Google API instead of IFTTT Webhooks

## SolarPHUMY2

v0.1 20200516
- code and measurement ever - voltage from solar panel over potential divider 3:1 every 2 minutes
- 2000 mAh battery lasts 18.5 days until June 4th

v0.2 20200615 
- included bootcount and temperature with LM35
- battery fully charged
- solar panel now 5 Volt instead of 6V, divider 2:1 and load 80 Ohm
