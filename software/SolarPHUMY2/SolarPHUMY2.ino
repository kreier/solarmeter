// Solarmeter 2 in Phu My Hung v0.3  2020/10/15
// future to measure power on solar panel 5V to 20V
//
// pin: 32, 33, 34, 35 - temp, current, solar2, LiPo2
// submit: solar2, current2, power, temp, LiPo2, bootCount2
 
#include <WiFi.h>
#include <Wire.h>
#include <soc/sens_reg.h>
#include <credentials_lib.h>  // this file contains:
// const char* ssid     = "REPLACE_WITH_YOUR_SSID";        // WiFi credentials
// const char* password = "REPLACE_WITH_YOUR_PASSWORD";
// const char* resource = "/trigger/data/with/key/value";  // IFTTT URL resource


RTC_DATA_ATTR int bootCount = 0;
static RTC_NOINIT_ATTR int reg_b; // place in RTC slow memory so available after deepsleep

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

// Time to sleep
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// sleep for 2 minutes = 120 seconds
uint64_t TIME_TO_SLEEP = 120;

//   32,      33,     34,    35
// temp, current, solar2, LiPo2  

int voltage[4] = {0, 0, 0, 0};    // all voltages in millivolt
int pins[4] = {32, 33, 34, 35};   // temp, current, solar2, LiPo2
int power = 0; // voltage measured times current measured - just to submit
int ledPin = 2;  // T-Koala: 5  DOIT DEVKIT V1: 2

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  
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

  String jsonObject = String("{\"value1\":\"") + voltage[2] + "|||" + voltage[1]
                          + "\",\"value2\":\"" + power      + "|||" + voltage[0]/10.0
                          + "\",\"value3\":\"" + voltage[3] + "|||" + bootCount + "\"}";

                      
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
  for(int i = 0; i < 4; i++) {
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
  //   32,      33,     34,    35  - 
  // temp, current, solar2, LiPo2  
  voltage[0] = int((voltage[0]) * 0.83 + 155);          // pin32 temp    voltage in millivolt/10 =  temperature in celsius
  voltage[1] = int((voltage[1]) * 0.83 + 155); // pin33 battery  voltage divider 10k : 1.2 k Ohm 1:1
  voltage[2] = int((voltage[2]) * 1.64 + 315);         // pin34 voltage solar divider 1k : 1k  2:1
  if(voltage[2] == 315) voltage[2] = 0;
  voltage[3] = int((voltage[3]) * 1.65 + 307);         // pin35 LiPo     voltage divider 10kOhm 2:1
  Serial.print("Boot number: ");
  Serial.println(bootCount);  
}
