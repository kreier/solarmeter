// Solar- and windmeter at AISVN v1.0 just rewrite 
// 2020/10/29 was 0.10
// 2024/01/12 renamed as 1.0
//
// pin definitions moved to the beginning of this file
//
// pin:       32,      33,       34,       35,   14,   26,   27,     12,   13
// value:  solar, battery, currentA, currentB, load, wind, temp, solar2, LiPo
//
// submit: solar, battery, current, power, load, wind, temp, solar2, LiPo, bootCount
//             0,       1,       2,     3,    4,    5,    6,      7,    8,

#define PIN_SOLAR    32
#define PIN_BATTERY  33
#define PIN_CURRENTA 34
#define PIN_CURRENTB 35
#define PIN_LOAD     14
#define PIN_WIND     26
#define PIN_TEMP      4 // was 27
#define PIN_SOLAR2   12
#define PIN_LIPO     13
#define PIN_LED       2

#define VALUE_SOLAR    0
#define VALUE_BATTERY  1
#define VALUE_CURRENTA 2
#define VALUE_CURRENTB 3
#define VALUE_LOAD     4
#define VALUE_WIND     5
#define VALUE_TEMP     6
#define VALUE_SOLAR2   7
#define VALUE_LIPO     8
#define VALUE_CURRENT  9
#define VALUE_POWER   10

const float scale_solar =   -7.52; // voltage divider 10k : 1.2 k Ohm 1:1
const float scale_battery = -7.52; // voltage divider 10k : 1.2 k Ohm 1:1
const float scale_currenta = 5.79; // voltage difference pin35 - pin34 x 8.4 is corrent (x0.804)
const float scale_currentb = 5.79;
const float scale_load =    -7.52; // voltage divider 10k : 1.2 k Ohm 1:1
const float scale_wind =    -7.52; // voltage divider 10k : 1.2 k Ohm 1:1
const float scale_temp =    0.804; // LM35 deliveres 10mV per Celsius
const float scale_solar2 =  4.583; // voltage divider 4.7k : 1k
const float scale_lipo =    1.608; // voltage divider 100kOhm 2:1

const int offset_solar =    29802;
const int offset_battery =  29802;
const int offset_currenta =     0;
const int offset_currentb =     0;
const int offset_load =     29802;
const int offset_wind =     29802;
const int offset_temp =       129;
const int offset_solar2 =     735;
const int offset_lipo =       258;

#include <WiFi.h>
#include <Wire.h>
#include <soc/sens_reg.h>

#include <credentials_aisvn.h> // WiFi credentials in separate file, it contains
// const char* ssid     = "REPLACE_WITH_YOUR_SSID";
// const char* password = "REPLACE_WITH_YOUR_PASSWORD";
// Replace with your unique IFTTT URL resource + uncomment
// const char* resource = "/trigger/data/with/key/value";

RTC_DATA_ATTR int bootCount = 0;
static RTC_NOINIT_ATTR int reg_b; // place in RTC slow memory so available after deepsleep

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

// Time to sleep
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// sleep for 2 minutes = 120 seconds
uint64_t TIME_TO_SLEEP = 120;

//    32,      33,       34,       35,   14,   26,   27,     12,   13
// solar, battery, currentA, currentB, load, wind, temp, solar2, LiPo  

int voltage[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       // all voltages in millivolt
int voltage2[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       // all voltages in millivolt
int pins[9] = {PIN_SOLAR, PIN_BATTERY, PIN_CURRENTA, PIN_CURRENTB, 
               PIN_LOAD, PIN_WIND, PIN_TEMP, PIN_SOLAR2, PIN_LIPO};  
// solar, battery, curA, curB, load, wind, temp, solar2, LiPo


void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);  
  Serial.begin(115200);
  // determine cause of reset
  esp_reset_reason_t reason = esp_reset_reason();
  Serial.print("reason ");Serial.println(reason);
  // get reg_b if reset not from deep sleep
  if ((reason != ESP_RST_DEEPSLEEP)) {
    reg_b = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG);
    Serial.println("Reading reg b.....");
  }
  Serial.print("reg b: ");
  printf("%" PRIu64 "\n", reg_b);
  delay(50);
  digitalWrite(PIN_LED, LOW);
  bootCount++;
  delay(1000);
  measureVoltages();

  digitalWrite(PIN_LED, HIGH);  
  initWifi();
  makeIFTTTRequest();
  digitalWrite(PIN_LED, LOW); 

  // enable timer deep sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);    
  Serial.println("Going to sleep now");
  // start deep sleep for 120 seconds (2 minutes)
  esp_deep_sleep_start();
}

void loop() {
  // sleeping so wont get here - as of now
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

  String jsonObject = String("{\"value1\":\"") + voltage[VALUE_SOLAR]/1000.0 
                                       + "|||" + voltage[VALUE_BATTERY]/1000.0 
                                       + "|||" + voltage[VALUE_CURRENT]/1000.0
                          + "\",\"value2\":\"" + voltage[VALUE_POWER]/1000.0 
                                       + "|||" + voltage[VALUE_LOAD]/1000.0
                                       + "|||" + voltage[VALUE_WIND]/1000.0
                          + "\",\"value3\":\"" + voltage[VALUE_TEMP]/10.0 
                                       + "|||" + voltage[VALUE_SOLAR2]/1000.0
                                       + "|||" + voltage[VALUE_LIPO]/1000.0
                          + "|||" + bootCount + "\"}";
                      
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
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, reg_b); // only needed after deep sleep
  SET_PERI_REG_MASK(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_DATA_INV);
  Serial.print(" ** Voltages measured: ");
  //
  // Measure the raw voltage values on 9 pins - multisample 100x
  for(int i = 0; i < 9; i++) {
    // multisample 100x to reduce noise - 9.5 microseconds x 100 = 1ms per voltage
    voltage[i] = 0;
    for(int multi = 0; multi < 100; multi++) {
      voltage[i] += analogRead( pins[i] );
    }
    voltage[i] = voltage[i] / 100;
    Serial.print(voltage[i]);
    Serial.print("  ");
  }
  // conversion to voltage prior to voltage divider
  convertVoltage(VALUE_SOLAR,    scale_solar,    offset_solar, true);
  convertVoltage(VALUE_BATTERY,  scale_battery,  offset_battery, false);
  convertVoltage(VALUE_CURRENTA, scale_currenta, offset_currenta, false);
  convertVoltage(VALUE_CURRENTB, scale_currentb, offset_currentb, false);
  convertVoltage(VALUE_LOAD,     scale_load,     offset_load, true);
  convertVoltage(VALUE_WIND,     scale_wind,     offset_wind, true);
  convertVoltage(VALUE_TEMP,     scale_temp,     offset_temp, false);
  convertVoltage(VALUE_SOLAR2,   scale_solar2,   offset_solar2, true);
  convertVoltage(VALUE_LIPO,     scale_lipo,     offset_lipo, false);
  voltage2[VALUE_CURRENT] = voltage[VALUE_CURRENTB] - voltage[VALUE_CURRENTA];
  voltage2[VALUE_POWER] = voltage2[VALUE_CURRENT] * voltage2[VALUE_SOLAR] / 1000;
  
  // pin:       32,      33,       34,       35,   14,   26,   27,     12,   13
  // value:  solar, battery, currentA, currentB, load, wind, temp, solar2, LiPo
  //
  // submit: solar, battery, current, power, load, wind, temp, solar2, LiPo, bootCount
  //             0,       1,       2,     3,    4,    5,    6,      7,    8,
  
  voltage[0] = int((4096 - voltage[0]) * 7.52 - 1000);  // pin32 solar    voltage divider 10k : 1.2 k Ohm 1:1
  if(voltage[0] < 0) voltage[0] = 0;
  voltage[1] = int((4096 - voltage[1]) * 7.52 - 1000);  // pin33 battery  voltage divider 10k : 1.2 k Ohm 1:1
  voltage[9] = int((voltage[3] - voltage[2]) * 5.79);   // voltage difference pin35 - pin34 x 8.4 is corrent (x0.804)
  voltage[10] = int(voltage[9] * voltage[0] / 1000);
  voltage[2] = int(voltage[2] * 5.79);
  voltage[3] = int(voltage[4] * 5.79);
  voltage[4] = int((4096 - voltage[4]) * 7.52 - 1000);  // pin14 load     voltage divider 10k : 1.2 k Ohm 1:1
  if(voltage[4] < 0) voltage[4] = 0;  
  voltage[5] = int((4096 - voltage[5]) * 7.52 - 1000);  // pin26 wind     voltage divider 10k : 1.2 k Ohm 1:1
  if(voltage[5] < 0) voltage[5] = 0;  
  voltage[6] = int((voltage[6]) * 0.804 + 129);         // pin27 temp     not connected yet
  voltage[7] = int((voltage[7]) * 4.583 + 735);         // pin12 Solar2   voltage divider 4.7k : 1k
  if(voltage[7] < 736) voltage[7] = 0;
  voltage[8] = int((voltage[8]) * 1.608 + 258);         // pin13 LiPo     voltage divider 100kOhm 2:1
  Serial.print("Boot number: ");
  Serial.println(bootCount);
  Serial.println("Now compare:");
  for(int i = 0; i < 11; i++) {
    Serial.print(voltage[i]);
    Serial.print("  -  ");
    Serial.println(voltage2[i]);
  }
}

void convertVoltage(int id, float scale, int offset, bool zero) {
  voltage2[id] = int(voltage[id] * scale + offset);
  if(zero == true) {
    if(voltage2[id] < 0) voltage2[id] = 0;
  }
}
