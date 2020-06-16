# Software for the solarmeter

We use the ESP32 to measure the voltages. It has sufficient ADC channels for all the different values. After submitting the data to the internet, the processor can go to deep sleep and save a lot of power.

## Known issues

The second ADC, ADC2 (the 10 pins 0, 2, 4, 12, 13, 14, 15, 25, 26, 27 according to [randomnerdtutorial](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/) does not work when WiFi is activated. Therefore we measure the data first, then switch on WiFi to submit the data.

Unfortunately it does not reactivate if the processor goes to deep sleep. After waking up these pin's can't be used. The workaround:

[Solution on github](https://github.com/espressif/arduino-esp32/issues/102#issuecomment-625542440)

## Actual code

```c
// Solar- and windmeter at AISVN - test edition
 
#include <WiFi.h>
#include <Wire.h>

RTC_DATA_ATTR int bootCount = 0;

// Replace with your SSID and Password
const char* ssid     = "ssid";  
const char* password = "password"; 

// Replace with your unique IFTTT URL resource
const char* resource = "/trigger/project/with/key/ofyours";

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

// Time to sleep
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// sleep for 2 minutes = 120 seconds
uint64_t TIME_TO_SLEEP = 120;

int voltage[8] = {0, 0, 0, 0, 0, 0, 0, 0};       // all voltages in millivolt
int pins[8] = {32, 33, 34, 35, 25, 26, 27, 13};   // solar, battery, load_1, load_2, LiPo, wind, dump
int ledPin = 5;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  
  Serial.begin(115200); 
  delay(50);
  digitalWrite(ledPin, LOW);
  bootCount++;
  delay(1000);
  measureVoltages();

  digitalWrite(ledPin, HIGH);  
  initWifi();
  makeIFTTTRequest();
  digitalWrite(ledPin, LOW); 

  // enable timer deep sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);    
  Serial.println("Going to sleep now");
  // start deep sleep for 120 seconds (2 minutes)
  esp_deep_sleep_start();
}

void loop() {
  // sleeping so wont get here 
}

// Establish a Wi-Fi connection with your router
void initWifi() {
  Serial.print("Connecting to: "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);  

  int timeout = 10 * 4; // 10 seconds
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() != WL_CONNECTED) {
     Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: "); 
  Serial.print(millis());
  Serial.print(", IP address: "); 
  Serial.println(WiFi.localIP());
}

// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() {
  Serial.print("Connecting to "); 
  Serial.print(server);
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);

  String jsonObject = String("{\"value1\":\"") + voltage[0] + "|||" + voltage[1] + "|||" + voltage[2]
                          + "\",\"value2\":\"" + voltage[3] + "|||" + voltage[4] + "|||" + voltage[5]
                          + "\",\"value3\":\"" + voltage[6] + "|||" + voltage[7] + "|||" +bootCount + "\"}";
                      
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
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop(); 
}

void measureVoltages() {
  Serial.print(" ** Voltages measured: ");
  for(int i = 0; i < 8; i++) {
    voltage[i] = analogRead( pins[i] );
    voltage[i] = int( voltage[i] * 0.826 + 150 );
    if( voltage[i] == 150 ) voltage[i] = 0;
    if( voltage[i] > 3300 ) voltage[i] = 3300;
    Serial.print(voltage[i]);
    Serial.print("  ");
  }
  voltage[0] = int((3300 - voltage[0]) * 9.4);  // pin34 solar  voltage divider 11kOhm 11:1
  voltage[1] = int((3300 - voltage[1]) * 9.4);  // pin34 solar  voltage divider 11kOhm 11:1
  voltage[7] = int(voltage[7] * 2);            // pin26 LiPo   voltage divider 100kOhm 2:1
  Serial.print("Boot number: ");
  Serial.println(bootCount);  
}
```

## Stations and software editions

All stations use an ESP32 with LiPo battery backup and build-in charger, reloaded with solar energy. Usually the charge controller only works when powered from USB, so a cable leaves the voltage regulator for 5V to the board.

### SolarAISVN

Main box at AISVN with a T-Koala since May 28th, 2020. It transmits webhook aisvn via IFTTT to IFTTT_aisvn every 120 seconds. That's 720 values each day or 262800 sets in a year. Pin assignment and submitted data:

```
pin:       32,      33,       34,       35,   14,   26,   27,     12,   13
value:  solar, battery, currentA, currentB, load, wind, temp, solar2, LiPo

submit:    solar, battery, current, load, wind, temp, solar2, LiPo, bootCount
voltage[]:     0,       1,       2,    4,    5,    6,      7,    8,
```
First column submitted is date and time, then the 8 values and finally the bootcount. 2,102,400 data points in a year.

### SolarAISVN2

The second box was added since the solar controllers have no common ground, but connect the positive voltage. This is equal to +3.3V of the first box and requires the second solar panel to actually charge the ESP32. With a second 12V lead battery connected to the hybrid controller, now with a smaller 18V solar panel and the wind generator we created a second measuring box. 

The board is a LOLIN32 lite from WeMos. Transmitting to IFTTT_aisvn2 every 120 seconds, the values are:

### SolarPHUMY

It is a copy of the AISVN box to further investigate possibilities and options. It features a T-Koala as well.

### SolarPHUMY2

This was the first setup from May 2020.

The board is a LOLIN32 lite from WeMos. Transmitting to IFTTT_aisvn2 every 120 seconds, the values are:
